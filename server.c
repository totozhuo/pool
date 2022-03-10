/*************************************************************************
    > File Name: server.c
# File Name: server.c
# Author : Mayanrong  
# QQ : 1684615293
# Email:1684615293@qq.com
# Created Time: 2022年03月01日 星期二 11时59分25秒
# 描述：                   
 ************************************************************************/

#include"head.h"
#include"function.h"

MYSQL mysql;
sem_t sem;

int main()
{
	int sockfd = 0,accfd = 0;
	struct sockaddr_in clientaddr;
	socklen_t len =  sizeof(clientaddr);
	int ret = 0;

//定义一个队列
	struct node *queue = NULL;
	queue = (struct node*)malloc(sizeof(struct node));
	sem_init(&sem,0,0);
	pthread_t th[4] = {0};

//创建sock套接字
	sockfd =  socket_init_sockfd(sockfd);
//预先产生5个子线程	
	for(int i = 0;i < 5;i++)
	{	
		ret = pthread_create(&th[i],NULL,thread,(void *)queue);
		if(ret != 0)
		{
			printf("error create\n");
			exit(-1);
		}
	}

	while(1)
	{
	//TCP服务器接受客户端的连接请求
		accfd = accept(sockfd,(struct sockaddr *)&clientaddr,&len);
		if(sockfd < 0)
		{
			perror("accpet");
			exit(-1);
		}

	// 客户端套接字进入队列
		push_queue(queue,accfd); 
		sem_post(&sem);	
	}

	close(sockfd);
	return 0;		
}
