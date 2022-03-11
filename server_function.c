/*************************************************************************
    > File Name: function.cpp
# File Name: function.cpp
# Author : Mayanrong  
# QQ : 1684615293
# Email:1684615293@qq.com
# Created Time: 2022年03月01日 星期二 10时53分28秒
# 描述：                   
 ************************************************************************/

#include"head.h"
#include"function.h"
	
extern MYSQL mysql;
extern sem_t sem;


int socket_init_sockfd(int sockfd)
{
	//创建套接字
	sockfd = socket(AF_INET,SOCK_STREAM,0);
	if(sockfd == -1)
	{
		perror("fail to socket");
		exit(1);
	}
	//填充服务器网络信息结构体  sockaddr_in
	struct sockaddr_in addr;
	bzero(&addr,sizeof(addr));
	socklen_t len = sizeof(addr);
	addr.sin_family = AF_INET;						//协议族，ipv4协议
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");	 //服务器IP地址
	addr.sin_port = 10086;							//服务器端口号
	
	//bind绑定服务器自己的地址
	int ret = bind(sockfd,(struct sockaddr *)&addr,len);
	if(ret == -1)
	{
		perror("fail to bind");
		exit(1);
	}
	//监听listen
	int reval = listen(sockfd,15);
	if(reval == -1)
	{
		perror("fail to listen");
		exit(1);
	}
	return sockfd;
}

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

void *thread(void *arg)
{
	int ret = 0;
	struct node *queue = (struct node *)arg;
	char buf[BUFSIZ] = {0};
	USE *use=NULL;
	HEAD *head=NULL;

	while(1)
	{
		sem_wait(&sem);
		int sockfd = pop_queue(queue);
		while(1)
		{
			memset(buf,0,sizeof(buf));
			ret = recv(sockfd,buf,sizeof(USE)+sizeof(HEAD),0);
			if(ret < 0)
			{
				perror("recv");
				exit(-1);
			}
			else if(ret==0)
			{
				close(sockfd);
				break;
			}

			head = (HEAD*)buf;
			use = (USE *)(buf + sizeof(HEAD));
			Line();
			switch (head->type)
			{
				case 0:
					strcpy(buf,"exit");
					Write(buf,sockfd);
					break;
				case 1:
					Inster(use,sockfd);
					break;	
				case 2:	
					Log_In_Find(use,sockfd);
					break;
				case 3:
					AddNemfood(use,sockfd);
					break;
				case 4:
					Addfood(use,sockfd);
					break;
				case 5:	
					Outfood(use,sockfd);
					break;
				case 6:
    					ClearFood(use,sockfd);
     					break;
			}
		}
		close(sockfd);
	}
	
}
//链接句柄
void Line()
{
	mysql_init(&mysql);
	MYSQL *flag=NULL;
	flag=mysql_real_connect(&mysql,"localhost","root","1","login",0,NULL,0);
	if(flag==NULL)
	{
		printf("%s\n",mysql_error(&mysql));
		exit(-1);
	}
}

//数据库插入
void Inster(USE *use,int accfd)
{
	char temp[1024]={0};
	sprintf(temp,"insert into pw values('%s','%s')",use->username,use->password);
	int f=mysql_query(&mysql,temp);
	if(f!=0)
	{
		memset(&temp,0,sizeof(temp));
		strcpy(temp,"用户名已被注册，请重新注册");
		Write(temp,accfd);
	}

	memset(&temp,0,sizeof(temp));
	strcpy(temp,"registered_success");
	Write(temp,accfd);
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
		{
			printf("f1:%s",mysql_error(&mysql));
		}
		memset(&temp,0,sizeof(temp));
		strcpy(temp,"进仓成功");
		Write(temp,accfd);
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
