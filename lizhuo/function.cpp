/***************************************************
> Copyright (C) 2022 ==lizhuo== All rights reserved.
> File Name: function.cpp
> Author: lizhuo
> Mail:1713147195@qq.com 
> Created Time: 2022年03月02日 星期三 18时28分32秒
***************************************************/

#include "hand.h"
#include "cJSON.h"
#include "threadpool.c"

using namespace std;

extern deque<DT> P;
extern MYSQL mysql;  
extern int iserver;

//链接数据库功能模块
void initDB()
{
	//链接数据库
	mysql_init(&mysql);   
	MYSQL *flag = NULL;   
	flag = mysql_real_connect(&mysql,"localhost","root","1","class120",0,NULL,0);
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
		/*
		pthread_create(&tid,NULL,do_work,NULL);//分离后不使用tid
		pthread_detach(tid);
		*/
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
	seraddr.sin_port = htons(atoi("10086"));
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
		DT node;
		accfd = accept(sockfd,(struct sockaddr*)&cliaddr,&len);
		if(accfd == -1)
		{
			perror("accept");
			exit(1);
		}
		else
		{
			threadpool_add_task(pool,do_work,(void*)&accfd);
			/*
			printf("data:%s\n",node.data);	
			node.addr = cliaddr;
			V.push_back(node);	
			*/
		}
	}
	
	threadpool_destroy(pool);
	close(sockfd);
	
	return NULL;
}

//工作函数
void* do_work(void* arg)
{
	int accfd = *(int*)arg;
	while(iserver)
	{
		/*
		if(V.size() <= 0)
		{
			usleep(100);
			continue;
		}
		DT node = V.front();
		V.pop_front();
		node.len = sizeof(node.addr);
		*/
		DT node;
		node.fd = accfd;
		REGISTER res;//账户结构体
		bzero(&res,sizeof(res));
		char buf[BUFSIZ] = {0};
		int n = read(accfd,buf,sizeof(buf));
		if(n == 0)
		{
			printf("the client closed\n");
			break;
		}
		
		json_string(&res,buf);
		if(res.id == 1)
		{
			RegisterFunc(&res,&node);
		}
		else if(res.id == 2)
		{
			LoginFunc(&res ,&node);
		}
	}
	pthread_exit(NULL);
}

//json转string功能模块
void json_string(REGISTER *res,char *data)
{
	cJSON *json = cJSON_Parse(data);
	cJSON *node = NULL;
	//查找目标值，返回非空
	node = cJSON_GetObjectItem(json,"id");
	if(node == NULL)
	{
		printf("id node == NULL\n");
	}
	else
	{
		printf("found id node\n");
		printf("value:%d\n",node->valueint);
		res->id = node->valueint;
	}
	node = cJSON_GetObjectItem(json,"name");
	if(node == NULL)
	{
		printf("name node == NULL\n");
	}
	else
	{
		printf("found name node\n");
		printf("value:%s\n",node->valuestring);
		strcpy(res->UserAccound,node->valuestring);
	}
	node = cJSON_GetObjectItem(json,"passwd");
	if(node == NULL)
	{
		printf("passwd node == NULL\n");
	}
	else
	{
		printf("found passwd node\n");
		printf("value:%d\n",node->valueint);
		res->PassWd = node->valueint;
	}

	cJSON_Delete(json);
}

//注册功能模块
void RegisterFunc(REGISTER *res ,DT *node)
{
	puts("5");
	//注册
	char sql[1000] = {0};
	sprintf(sql,"insert into list values('%s',%d)",res->UserAccound,res->PassWd);
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
			int f = mysql_real_query(&mysql,sql.data,strlen(sql.data));      
			if(f!=0)
			{
				printf("%s\n",mysql_error(&mysql));
				exit(1);
			}
			else
				printf("insert success!\n");
			write(sql.fd,"regist success",30*sizeof(char));
		}
	}
	pthread_exit(NULL);
}

//登录功能模块
void LoginFunc(REGISTER *res ,DT *node)
{
	//查询
	char sql[1000] = {0};
	sprintf(sql,"select * from list where account = '%s' and password = %d",res->UserAccound,\
			res->PassWd);
	puts(sql);	

	int f = mysql_query(&mysql,sql);    
	if(f!=0)
	{
		printf("%s\n",mysql_error(&mysql));
		exit(1);
	}

	//缓冲查询结果
	MYSQL_RES *resl = NULL;         
	resl = mysql_store_result(&mysql); 
	if(resl == NULL)
	{
		printf("%s\n",mysql_error(&mysql));
		exit(1);
	}

	my_ulonglong row_l = 0;
	row_l = mysql_num_rows(resl);       	
	if(row_l > 0)
		write(node->fd,"login success",30*sizeof(char));
	else
		write(node->fd,"login fail",30*sizeof(char));
	mysql_free_result(resl);
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
