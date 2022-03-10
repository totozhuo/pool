/***************************************************
  > Copyright (C) 2022 ==KINGYI== All rights reserved.
  > File Name: udp_server.c
  > Author: LV
  > Mail:joseph2009@163.com 
  > Created Time: 2022年01月13日 星期四 23时20分10秒
 ***************************************************/

#include "hand.h"

int main(int argc,const char* argv[])
{
	int sockfd = 0;
	//create socket link
	sockfd = socket(AF_INET,SOCK_STREAM,0); //协议族,UDP,默认(自定义)
	if(sockfd == -1)
	{
		perror("socket");
		exit(1);
	}
	else
	{
		printf("sockfd:%d\n",sockfd);
	}

	struct sockaddr_in seraddr;//替代结构体
	bzero(&seraddr,sizeof(seraddr));

	seraddr.sin_family = AF_INET;//协议族
	seraddr.sin_port = htons(atoi(argv[2]));//端口号
	seraddr.sin_addr.s_addr = inet_addr(argv[1]);//IP地址,转网络字节序

	socklen_t len = sizeof(seraddr);

	int ret = connect(sockfd,(struct sockaddr*)&seraddr,len);
	if(ret != 0)
	{
		perror("connect");
		exit(1);
	}

	char* con = (char*)malloc(sizeof(HEAD)+sizeof(BODY));
	HEAD head;
	BODY body;
	char buf[BUFSIZ] = {0};

	while(1)
	{
		memset(con,0,sizeof(HEAD)+sizeof(BODY));

		//登录例子：type == 1   
		//初始化完成：connect --- sockfd
		//数据赋值  值放到结构体中
		//Head.type = 1; --- 判断类型  进入不同的模块 
		puts("input type:");
		scanf("%d",&head.type);
		if(head.type == 1)
		{
			puts("input accout password");
			scanf("%s",body.UserAccount);
			scanf("%s",body.PassWd);
		}
		else if(head.type == 2)
		{
			puts("input id name age accout password");
			scanf("%d",&body.id);
			scanf("%s",body.name);
			scanf("%d",&body.age);
			scanf("%s",body.UserAccount);
			scanf("%s",body.PassWd);
		}
		//值放到空间 – send  把结构体放到堆区中
		memcpy(con,&head,sizeof(HEAD));
		memcpy(con+sizeof(HEAD),&body,sizeof(BODY));

		//Send() – server
		send(sockfd,con,sizeof(HEAD)+sizeof(BODY),0);   //---- 服务器

		read(sockfd,buf,sizeof(buf));
		printf("%s\n",buf);
	}

	free(con);
	close(sockfd);

	return 0;
}
