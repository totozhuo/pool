/*************************************************************************
    > File Name: other_function.c
# File Name: other_function.c
# Author : 李卓，苟涛，马彦荣 
# QQ : 1684615293
# Email:1684615293@qq.com
# Created Time: 2022年03月10日 星期四 14时06分47秒
# 描述：                   
************************************************************************/
 
#include "head.h"
#include "function.h"
#include "threadpool.c"

using namespace std;
	
extern deque<DT> P;
extern MYSQL mysql;  
extern int iserver;
int count = 0;
extern OP op;

//链接数据库功能模块
void initDB()
{
	//链接数据库
	mysql_init(&mysql);   
	MYSQL *flag = NULL;   
	flag = mysql_real_connect(&mysql,"localhost","root","1","login",0,NULL,0);
	if(flag == NULL)
	{
		printf("%s\n",mysql_error(&mysql));
		exit(1);
	}
	printf("link successful\n");
}

//启动服务器模块
void RunIMServer()
{
	if(iserver)
	{
		printf("服务器正在运行....\n");
	}
	else
	{
		iserver = 1;
		//创建线程池，最小线程10，最大线程100，最多任务数100	
		threadpool_t *pool = threadpool_create(10,100,100);
		pthread_t tid = 0;
		//创建写入线程
		pthread_create(&tid,NULL,write_work,NULL);//分离后不使用tid
		pthread_detach(tid);
		//创建接收线程
		pthread_create(&tid,NULL,recv_work,(void*)pool);//分离后不使用tid
		pthread_detach(tid);
		printf("服务器已启动....\n");	
	}
}

//recv线程
void* recv_work(void* arg)
{
	threadpool_t *pool = (threadpool_t*)arg;
	
	//create socket link
	int sockfd = 0;
	sockfd = socket(AF_INET,SOCK_STREAM,0);
	if(sockfd == -1)
	{
		perror("socket");
		exit(1);
	}
	else
	{
		printf("sockfd:%d\n",sockfd);
	}

	//bind --- IP +Port+AF_INET
	int reval = 0;
	struct sockaddr_in seraddr; 
	bzero(&seraddr,sizeof(seraddr));

	seraddr.sin_family = AF_INET;
	seraddr.sin_port = htons(atoi("10085"));
	seraddr.sin_addr.s_addr = inet_addr("127.1");
	socklen_t len = sizeof(seraddr);

	reval = bind(sockfd,(struct sockaddr*)&seraddr,len);
	if(reval == -1)
	{
		perror("bind");

		exit(1);
	}
	else
	{
		printf("socket init success!\n");
	}

	listen(sockfd,10);
	//recv--client

	struct sockaddr_in cliaddr;
	bzero(&cliaddr,len);
	int accfd = 0;
	
	while(iserver)
	{
		accfd = accept(sockfd,(struct sockaddr*)&cliaddr,&len);
		if(accfd == -1)
		{
			perror("accept");
			exit(1);
		}
		else
		{
			printf("心跳检测线程已开启!\n");
			pthread_t tid = 0;
			pthread_create(&tid,NULL,check_handler,(void*)&accfd);//分离后不使用tid
			pthread_detach(tid);

			threadpool_add_task(pool,do_work,(void*)&accfd);
		}
	}
	
	threadpool_destroy(pool);
	close(sockfd);
	
	return NULL;
}

void heart_check()
{
	count = 0;	
}

void* check_handler(void* arg)
{
	int accfd = *(int*)arg;
	while(1)
	{
		count++;
		sleep(1);
		if(count == 5)
		{
			printf("用户已掉线!!\n");
			close(accfd);
			break;
		}
	}
	pthread_exit(NULL);
}

//工作函数
void* do_work(void* arg)
{
	int sockfd = *(int*)arg;
	while(iserver)
	{
		DT node;
		node.fd = sockfd;
		USE *use=NULL;
		HEAD *head=NULL;
		char buf[BUFSIZ] = {0};
		
		int n = read(sockfd,buf,sizeof(buf));
		if(n == 0)
		{
			close(sockfd);
			printf("the client closed\n");
			break;
		}
		
		head = (HEAD*)buf;
		use = (USE *)(buf + sizeof(HEAD));
		
		switch (head->type)
		{
			case 0:break;
			case 1:
				RegisterFunc(use,&node);break;	
			case 2:	
				Log_In_Find(use,sockfd);break;
			case 3:
				AddNemfood(use,sockfd);break;
			case 4:
				Addfood(use,sockfd);break;
			case 5:	
				Outfood(use,sockfd);break;
			case 6:
				ClearFood(use,sockfd);break;
			case 7:
				Findfood(use,sockfd);break;
			case 8:
				SmartInfood(use,sockfd);break;
			case 9:
				Allotfood(use,sockfd);break;
			case 10:
				heart_check();break;
		}
	}
	
	close(sockfd);
	pthread_exit(NULL);
}

//注册功能模块
void RegisterFunc(USE *use ,DT *node)
{
	//注册
	char sql[1000] = {0};
	sprintf(sql,"insert into pw values('%s','%s')",use->username,use->password);
	strcpy(op.name,use->username);
	strcpy(op.type,"注册");
	Oper();
	puts(sql);	 
	strcpy(node->data,sql);	
	P.push_back(*node);
}

//mysql写入线程
void* write_work(void* arg)
{
	while(iserver)
	{
		if(P.size() > 0)
		{
			DT sql = P.front();
			P.pop_front();
			//执行sql语句
			char temp[1024]={0};
			int f = mysql_real_query(&mysql,sql.data,strlen(sql.data));      
			if(f!=0)
			{
				memset(&temp,0,sizeof(temp));
				strcpy(temp,"用户名已被注册，请重新注册");
				Write(temp,sql.fd);
				printf("%s\n",mysql_error(&mysql));
				exit(1);
			}
			memset(&temp,0,sizeof(temp));
			strcpy(temp,"registered_success");
			Write(temp,sql.fd);
		}
	}
	pthread_exit(NULL);
}

//监控过期产品线程
void* listen_work(void* arge)
{
	TIME *arg = (TIME*)arge;
	sleep(arg->time);
	char tmp[1024] = {0};
	sprintf(tmp,"%s过期%d箱,请报废",arg->name,arg->all_count);
	write(arg->fd,tmp,sizeof(tmp));
	free(arg);
	pthread_exit(NULL);
}

//关闭服务器模块
void StopIMServer()
{
	if(iserver)
	{
		iserver = 0;
		printf("服务器已关闭....\n");
	}
	else
	{
		printf("当前服务器已处于关闭状态....\n");
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
