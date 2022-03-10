/***************************************************
> Copyright (C) 2022 ==KINGYI== All rights reserved.
> File Name: udp_server.c
> Author: LV
> Mail:joseph2009@163.com 
> Created Time: 2022年01月13日 星期四 23时20分10秒
***************************************************/

#include "hand.h"
#include "cJSON.h"

int main(int argc,const char* argv[])
{
	int sockfd = 0;
	//create socket link
	sockfd = socket(AF_INET,SOCK_STREAM,0); //协议族,TCP,默认(自定义)
	if(sockfd == -1)
	{
		perror("socket");
		exit(1);
	}
	else
	{
		printf("sockfd:%d\n",sockfd);
	}
	//定义对方IP
	struct sockaddr_in seraddr;//替代结构体
	bzero(&seraddr,sizeof(seraddr));

	seraddr.sin_family = AF_INET;//协议族
	seraddr.sin_port = htons(atoi("10086"));//端口号
	seraddr.sin_addr.s_addr = inet_addr("127.1");//IP地址,转网络字节序

	socklen_t len = sizeof(seraddr);
	int ret = connect(sockfd,(struct sockaddr*)&seraddr,len);	
		if(ret != 0)
		{
			perror("connect");
			exit(1);
		}

	//recv--client
	struct sockaddr_in cliaddr;
	bzero(&cliaddr,len);
	
	ssize_t rcv = 0;
	char buf[100] = {0};
	
	REGISTER rst;//客户端结构体
	bzero(&rst,sizeof(rst));
	while(1)
	{
		bzero(&rst,sizeof(rst));
		puts("input id name and passwd:");
		scanf("%d%s%d",&rst.id,rst.UserAccound,&rst.PassWd);
		//创建一个空的目标
		cJSON *json = cJSON_CreateObject();
		//添加一个键值给目标
		cJSON_AddNumberToObject(json,"id",rst.id);
		cJSON_AddItemToObject(json,"name",cJSON_CreateString(rst.UserAccound));	
		cJSON_AddNumberToObject(json,"passwd",rst.PassWd);
		char *json_data = NULL;
		printf("data:%s\n",json_data = cJSON_Print(json));
		
		int sd = write(sockfd,json_data,BUFSIZ);
		if(sd < 0)
		{
			perror("write");
			exit(1);
		}
		else
		{
			printf("\twrite success!\n");
		}
		free(json_data);
		//将JSON结构所占用的数据空间释放
		cJSON_Delete(json);

		bzero(buf,sizeof(buf));
		rcv = read(sockfd,buf,sizeof(buf));
		if(rcv == -1)
		{
			perror("read");
			exit(1);
		}
		else
		{
			printf("server:%s---%ld\n",buf,rcv);
		}

	}
	
	close(sockfd);
	
	return 0;
}
