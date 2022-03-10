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
//获取当地时间
char* Time()
{
	time_t t=time(NULL);
	struct tm *p=localtime(&t);
	static char buf[100]={0};
	sprintf(buf,"%4d-%2d-%2d-%02d:%02d:%02d",p->tm_year+1900,p->tm_mon+1,
			p->tm_mday,p->tm_hour,p->tm_min,p->tm_sec);

	return buf;
}

//错误日志
void Slip(char *p)
{
	int fd=0,ret=0;
	char str[1024]={0};

//错误信息采集
	strcpy(str,p);
	strcat(str,"错误原因：\t");
	strcat(str,strerror(errno));
	strcat(str,"发生时间：\t");
	strcat(str,Time());
	strcat(str,"\n");

//打开错误日志文件
	fd=open("./error.txt",O_RDWR|O_CREAT|O_APPEND,0644);
	if (fd<0)
	{

		perror("open");
		exit(-1);
	}

	ret=write(fd,str,strlen(str));
	if (ret<0)
	{
		perror("write");
		exit(-1);
	}
	close(fd);
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

//注册
void Sign_In(HEAD *p,int accfd)
{
	char buf[BUFSIZ]={0};
	HEAD head;//选择功能
	memset(&head,0,sizeof(head));
	
	USE use; //账号密码
	memset(&use,0,sizeof(use));
	head.type=p->type;
	
	puts("请输入需要注册的用户名,大于三位数");
	while(1)
	{
		scanf("%s",use.username);
		if(strlen(use.username)>2)
			break;
		else
			puts("用户名过于简单，请重新输入");
	}		
		
	puts("请设置密码，密码必须大于6位");
	while(1)
	{
		scanf("%s",use.password);
		if(strlen(use.password)>5)
			break;
		else
			puts("密码过于简单，请重新输入");		
	}	
	memcpy(buf,&head,sizeof(head));
	memcpy(buf+sizeof(head),&use,sizeof(use));
	Write(buf,accfd);
}

void Register(HEAD *p,int accfd)
{
	char buf[1024]={0};
	HEAD head;//选择功能
	memset(&head,0,sizeof(head));
	
	USE use; //账号密码
	memset(&use,0,sizeof(use));

	head.type=p->type;
	puts("请输入用户名");
	scanf("%s",use.username);
	puts("请输入密码");
	scanf("%s",use.password);
	memcpy(buf,&head,sizeof(head));
	memcpy(buf+sizeof(head),&use,sizeof(use));
	Write(buf,accfd);
}

//队列插入
void push_queue(struct node *queue,int data)
{
	struct node *p = queue;
	struct node *q = (struct node *)malloc(sizeof(struct node));
	memset(q,0,sizeof(struct node));
	q->data = data;

	while((p->next) != NULL)
	{
		p = p->next;
	}
	p->next = q;
	q->next = NULL;
}
//删除队列元素
int pop_queue(struct node *queue)
{
	int data = 0;
	struct node *p = NULL;
	p = queue->next;
	data = p->data;
	queue->next = p->next;
	free(p);

	return data;
}

//判断队列是否为空
int is_empty(struct node *queue)
{
	struct node *p = queue->next;
	if(p == NULL)
		return 1;
	else
		return 0;
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

			head = (HEAD*)buf;
			use = (USE *)(buf + sizeof(HEAD));
			Line();
			switch (head->type)
			{
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
			}
		}
		close(sockfd);
	}
}

void  AddNemfood(USE *use,int accfd)
{
	char temp[1024] = {0}; 
	char buf[1024] = {0}; 
	sprintf(temp,"insert into kind values('%s',%d,%d,%d,%d)",use->name,use->count,use->day,use->place,use->putgoods);
	int f=mysql_query(&mysql,temp);
	if(f!=0)
	{
		memset(&temp,0,sizeof(temp));
		strcpy(temp,"新产品录入失败");
		Write(temp,accfd);
	}
	else
	{
		sprintf(buf,"create table %s(batch int PRIMARY KEY AUTO_INCREMENT,name varchar(30),count int,time1 int,time2 int)",use->name);
			puts(buf);
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
//进仓
void  Addfood(USE *use,int accfd)
{
	char temp[1024] = {0};
   	time_t t = time(NULL);
	int time = t;
	sprintf(temp,"insert into %s values(%d,'%s',%d,%d,%d)",use->name,0,use->name,use->count,time,0);
	int f=mysql_query(&mysql,temp);
	if(f!=0)
	{
		memset(&temp,0,sizeof(temp));
		strcpy(temp,"进仓失败");
		Write(temp,accfd);
	}
	else
	{
		memset(&temp,0,sizeof(temp));
		strcpy(temp,"进仓成功");
		Write(temp,accfd);
	}
}

void Input_Newfoot(USE *use, HEAD *head,int accfd)
{
	char buf[1024] = {0};
	printf("请输入品类名称：\n");
	scanf("%s",use->name);
	printf("请输入商品数量：\n");
	scanf("%d",&use->count);
	printf("请输入该商品保质期：\n");
	scanf("%d",&use->day);
	printf("请输入该商品入库位置：\n");
	scanf("%d",&use->place);
	use->putgoods = 0;
	memcpy(buf,head,sizeof(HEAD));
	memcpy(buf+sizeof(HEAD),use,sizeof(USE));
	Write(buf,accfd);
}
//进仓
void Input_Foot(USE *use, HEAD *head,int accfd)
{
	char buf[1024] = {0};
	printf("请输入品类名称：\n");
	scanf("%s",use->name);
	printf("请输入商品数量：\n");
	scanf("%d",&use->count);
	memcpy(buf,head,sizeof(HEAD));
	memcpy(buf+sizeof(HEAD),use,sizeof(USE));
	Write(buf,accfd);
}
//出仓客户端
void Output_Foot(USE *use, HEAD *head,int accfd)
{
	char buf[1024] = {0};
	printf("请输入出货品类名称：\n");
	scanf("%s",use->name);
	printf("请输入出货商品数量：\n");
	scanf("%d",&use->count);
	memcpy(buf,head,sizeof(HEAD));
	memcpy(buf+sizeof(HEAD),use,sizeof(USE));
	Write(buf,accfd);
}
//出仓服务器
/*
void  Outfood(USE *use,int accfd)
{
	char temp[1024] = {0};
   	time_t t = time(NULL);
	int time = t;
	
	sprintf(temp," into %s values(%d,'%s',%d,%d,%d)",use->name,0,use->name,use->count,time,0);
	int f=mysql_query(&mysql,temp);
	if(f!=0)
	{
		memset(&temp,0,sizeof(temp));
		strcpy(temp,"进仓失败");
		Write(temp,accfd);
	}
	else
	{
		memset(&temp,0,sizeof(temp));
		strcpy(temp,"进仓成功");
		Write(temp,accfd);
	}
}
*/

