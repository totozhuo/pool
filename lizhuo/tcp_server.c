/***************************************************
> Copyright (C) 2022 ==KINGYI== All rights reserved.
> File Name: udp_server.c
> Author: LV
> Mail:joseph2009@163.com 
> Created Time: 2022年01月13日 星期四 23时20分10秒
***************************************************/

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <time.h>

int main(int argc,const char* argv[])
{
	//create socket link
	int sockfd1 = 0;
	
	sockfd1 = socket(AF_INET,SOCK_STREAM,0); //协议族,TCP,默认(自定义)
	if(sockfd1 == -1)
	{
		perror("socket");
		exit(1);
	}
	else
	{
		printf("sockfd:%d\n",sockfd1);
	}

	//bind --- IP +Port+AF_INET
	int reval = 0;
	struct sockaddr_in seraddr,cliaddr;//替代结构体
	socklen_t len;
	bzero(&seraddr,sizeof(seraddr));

	seraddr.sin_family = AF_INET;//协议族
	seraddr.sin_port = htons(atoi(argv[2]));//端口号
	seraddr.sin_addr.s_addr = inet_addr(argv[1]);//IP地址,转网络字节序
	len = sizeof(seraddr);
	
	reval = bind(sockfd1,(struct sockaddr*)&seraddr,len);//套接字描述符，协议信息，addr结构体长度
	if(reval == -1)
	{
		perror("bind");
		exit(1);
	}
	else
	{
		printf("socket init success!\n");
	}


	listen(sockfd1,10);
	
	//服务器接收与发送
	int sockfd2 = 0;
	int ret = 0;
	char buf[BUFSIZ] = {0};
    
	sockfd2 = accept(sockfd1,(struct sockaddr*)&cliaddr,&len);//0,接收对方主机的网络地址
	
	while(1)
	{
		bzero(buf,sizeof(buf));
		ret = read(sockfd2,buf,sizeof(buf));
		write(1,buf,ret);

		if(strncmp(buf,"bye",3)==0)
			break;
		
		for(int i = 0;i < ret; i++)
			buf[i] += 1;

		write(sockfd2,buf,ret);
	}
	
	close(sockfd1);
	close(sockfd2);
	
	return 0;
}
