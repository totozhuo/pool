/*************************************************************************
    > File Name: function.cpp
# File Name: function.cpp
# Author : Mayanrong  
# QQ : 1684615293
# Email:1684615293@qq.com
# Created Time: 2022年03月01日 星期二 10时53分28秒
# 描述：                   
 ************************************************************************/

#include "head.h"
#include "function.h"

using namespace std;
extern MYSQL mysql;

//读取文件
int Read(int sockfd)
{
		int ret=0;
		char buf[1024]={0};
		ret=read(sockfd,buf,1023);
		if(ret<0)
		{
			memset(buf,0,sizeof(buf));
			sprintf(buf,"文件名：%s \t 函数名：%s \t 行号：%d\t",__FILE__,
					__FUNCTION__,__LINE__);
			Slip(buf);
			exit(1);	
		}

		puts(buf);

		if(strcmp(buf,"log_in_success")==0)
			return 0;
		if(strcmp(buf,"exit")==0)
			return -1;
		return 2;
}	

//向服务器发送文件
void Write(char *buf,int sockfd)
{
	int ret=0;
	char temp[1024]={0};
	ret=write(sockfd,buf,sizeof(USE)+sizeof(HEAD));
	if(ret<0)
	{
		memset(temp,0,sizeof(temp));
		sprintf(temp,"文件名：%s \t 函数名：%s \t 行号：%d\t",__FILE__,
				__FUNCTION__,__LINE__);
		Slip(temp);
		exit(1);	
	}
}

void Log_In_Find(USE *use,int accfd)
{
	char m[1024]={0};
	sprintf(m,"select *from pw where username = '%s' and password = '%s'",use->username,use->password);
	int f=mysql_query(&mysql,m);
	if(f!=0)
	{
		printf("m:%s\n",mysql_error(&mysql));
		exit(1);
	}

	MYSQL_RES *res=NULL;
	MYSQL_ROW row;

	res=mysql_store_result(&mysql);
	if(res==NULL)
	{
		printf("res:%s\n",mysql_error(&mysql));
		exit(1);
	}
	
	row=mysql_fetch_row(res);
	if (row>0)
	{
		memset(m,0,sizeof(m));
		strcpy(m,"log_in_success");
		Write(m,accfd);
	}
	else
	{
		memset(m,0,sizeof(m));
		strcpy(m,"登录失败");
		Write(m,accfd);
	}
}

//新产品录入
void  AddNemfood(USE *use,int accfd)
{
	char temp[1024] = {0}; 
	char buf[1024] = {0}; 
	sprintf(temp,"insert into kind values('%s',%d,0,%d,%d,%d)",use->name,use->expiration_day,use->all_count,use->put_count,use->remain_count);
	int f=mysql_query(&mysql,temp);
	if(f!=0)
	{
		memset(&temp,0,sizeof(temp));
		strcpy(temp,"新产品录入失败");
		Write(temp,accfd);
		printf("f:%s\n",mysql_error(&mysql));
	}
	else
	{
		sprintf(buf,"create table %s(batch int PRIMARY KEY AUTO_INCREMENT,name varchar(30),all_count int,purchase_time int,shipping_time int)",use->name);
		int f=mysql_query(&mysql,buf);
		if(f != 0)
		{
			printf("f:%s",mysql_error(&mysql));
		}
		memset(&temp,0,sizeof(temp));
		strcpy(temp,"新产品录入成功");
		Write(temp,accfd);
	}
}

//进仓
void  Addfood(USE *use,int accfd)
{
	char temp[1024] = {0};
   	time_t t = time(NULL);
	use->purchase_time = t;
	sprintf(temp,"insert into %s values(0,'%s',%d,%d,%d)",use->name,use->name,use->all_count,use->purchase_time,0);
	int f=mysql_query(&mysql,temp);
	if(f!=0)
	{
		printf("f:%s",mysql_error(&mysql));
		memset(&temp,0,sizeof(temp));
		strcpy(temp,"进仓失败");
		Write(temp,accfd);
	}
	else
	{
		memset(&temp,0,sizeof(temp));
		sprintf(temp,"update kind set all_count = all_count+%d , remain_count= all_count - put_count where name='%s'",use->all_count,use->name);
		f=mysql_query(&mysql,temp);
		if(f!=0)
		{	char m[1024]={0};
	sprintf(m,"select *from pw where username = '%s' and password = '%s'",use->username,use->password);
	int f=mysql_query(&mysql,m);
	if(f!=0)
	{
		printf("m:%s\n",mysql_error(&mysql));
		exit(1);
	}

	MYSQL_RES *res=NULL;
	MYSQL_ROW row;

	res=mysql_store_result(&mysql);
	if(res==NULL)
	{
		printf("res:%s\n",mysql_error(&mysql));
		exit(1);
	}
	
	row=mysql_fetch_row(res);
	if (row>0)
	{
		memset(m,0,sizeof(m));
		strcpy(m,"log_in_success");
		Write(m,accfd);
	}
	else
	{
		memset(m,0,sizeof(m));
		strcpy(m,"登录失败");
		Write(m,accfd);
	}

			printf("f1:%s",mysql_error(&mysql));
		}
		memset(&temp,0,sizeof(temp));
		strcpy(temp,"进仓成功");
		Write(temp,accfd);
		
		char sql[1024]={0};
		sprintf(sql,"select *from kind where username = '%s'",use->username);
		int f=mysql_query(&mysql,sql);
		if(f!=0)
		{
			printf("m:%s\n",mysql_error(&mysql));
			exit(1);
		}

		MYSQL_RES *res=NULL;
		MYSQL_ROW row;

		res=mysql_store_result(&mysql);
		if(res==NULL)
		{
			printf("res:%s\n",mysql_error(&mysql));
			exit(1);
		}

		int time;
		row=mysql_fetch_row(res);
		if (row>0)
			time = atoi(row[1]);
		
		//accfd time use->name use->all_count	
		TIME *message = (TIME*)malloc(sizeof(TIME));
		message->fd = accfd;
		message->time = time;
		strcpy(message->name,use->name);
		message->all_count = use->all_count;

		//创建监控线程
		pthread_t tid = 0;
		pthread_create(&tid,NULL,listen_work,(void*)message);//分离后不使用tid
		pthread_detach(tid);
	}
}

//出仓服务器
void  Outfood(USE *use,int accfd)
{ 
	time_t t = time(NULL);
	int time = t;
	USE buf[1024] = {0};
	char temp[1024] = {0};

	sprintf(temp,"select *from %s",use->name);
	int f = mysql_query(&mysql,temp);
	if( f!= 0)
	{
		printf("%s\n",mysql_error(&mysql));
		exit(1);
	}

	MYSQL_RES *res = NULL;
	MYSQL_ROW row;
	int i = 0, put =0;
	put = use->put_count;
	res = mysql_store_result(&mysql);
	if(res == NULL)
	{
		printf("%s\n",mysql_error(&mysql));
		exit(1);
	}

	my_ulonglong row_l = 0; 
	row_l = mysql_num_rows(res); 
	while((row = mysql_fetch_row(res)) != NULL)
	{
		buf[i].all_count = atoi(row[2]);
		buf[i].batch = atoi(row[0]);
		i = i + 1;
	}
	
	for(i = 0;i < row_l;i++)
	{
		if(use->put_count >= buf[i].all_count)//判断出货量是否大于第一批次货物的总量
		{
			use->put_count = use->put_count - buf[i].all_count;
			bzero(temp,sizeof(temp));
			sprintf(temp,"update %s set all_count=%d,shipping_time=%d where batch=%d",use->name,0,time,buf[i].batch);
			mysql_query(&mysql,temp);
		}
		else
			break;
	}

	if(use->put_count < buf[i].all_count)
	{
		bzero(temp,sizeof(temp));
		int s = buf[i].all_count - use->put_count;
		sprintf(temp,"update %s set all_count=%d,shipping_time=%d where batch=%d ",use->name,s,time,buf[i].batch);
		mysql_query(&mysql,temp);

		memset(&temp,0,sizeof(temp));
		strcpy(temp,"出仓成功");
		Write(temp,accfd);
	}
		
	bzero(temp,sizeof(temp));
	sprintf(temp,"update kind set put_count=put_count+%d,remain_count= all_count-put_count where name='%s'",put,use->name);
	mysql_query(&mysql,temp);
}

//旧产品下线服务器
void ClearFood(USE *use,int accfd)
{
	char temp[1024] = {0}; 
	char buf[1024] = {0}; 
	sprintf(temp,"drop table %s",use->name);

	int ret = mysql_query(&mysql,temp);
	if(ret != 0)
	{
		memset(&temp,0,sizeof(temp));
		strcpy(temp,"产品下线失败");
		Write(temp,accfd);
		printf("f:%s\n",mysql_error(&mysql));
	}

	sprintf(buf,"delete from kind where name='%s'",use->name);
	int f = mysql_query(&mysql,buf);
	if(f!=0)
	{
		memset(&temp,0,sizeof(temp));
		strcpy(temp,"产品下线失败");
		Write(temp,accfd);
		printf("f:%s\n",mysql_error(&mysql));
	}
}
