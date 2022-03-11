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
	printf("link succeed\n");

	char buf[BUFSIZ] = {0};
	char buf1[BUFSIZ] = {0};
	/*
	int fd = open("./dem.txt",O_RDWR|O_CREAT,0777);	
	if(fd == -1)
	{
		perror("open");
		exit(1);
	}
	*/
	while(1)
	{
		/*
		int ret = read(fd,buf,sizeof(buf));
		if(ret == 0)
			break;
		*/
		printf("请输入一串字母，服务器将其+1\n");
		scanf("%s",buf);
		write(sockfd,buf,sizeof(buf));	
		
		read(sockfd,buf1,sizeof(buf1));
		printf("%s\n",buf1);
	}
	
	close(sockfd);
	//close(fd);
	
	return 0;
}
