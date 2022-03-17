/*************************************************************************
    > File Name: staff_client.cpp
# File Name: staff_client.cpp
# Author : Mayanrong  
# QQ : 1684615293
# Email:1684615293@qq.com
# Created Time: 2022年03月11日 星期五 20时52分19秒
# 描述：                   
 ************************************************************************/

#include "head.h"
#include "function.h"

using namespace std;
	
deque<DT> P;
MYSQL mysql;  
int iserver;

int main()
{
	int sockfd=0;
	int	ret=0;
	socklen_t len=0;
	char buf[1024]={0};

	struct sockaddr_in addr;
	bzero(&addr,sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(atoi("10086"));
	addr.sin_addr.s_addr = inet_addr("123.57.173.152");
	len = sizeof(addr);

	sockfd=socket(AF_INET,SOCK_STREAM,0);
	if(sockfd<0)
	{
		memset(buf,0,sizeof(buf));
		sprintf(buf,"文件名：%s \t 函数名：%s \t 行号：%d\t",__FILE__,
				__FUNCTION__,__LINE__);
		Slip(buf);
		exit(1);
	}
	//发送链接请求
	
	ret = connect(sockfd,(struct sockaddr*)&addr,len);
	if(ret < 0)
	{
		printf("link fail\n");
		memset(buf,0,sizeof(buf));
		sprintf(buf,"文件名：%s \t 函数名：%s \t 行号：%d\t",__FILE__,
				__FUNCTION__,__LINE__);
		Slip(buf);
		exit(1);	
	}
	
	//创建心跳包线程
	pthread_t tid = 0;
	pthread_create(&tid,NULL,heart_work,(void*)&sockfd);//分离后不使用tid
	pthread_detach(tid);

	pthread_create(&tid,NULL,Rd,(void*)&sockfd);//分离后不使用tid
	pthread_detach(tid);
	
	HEAD head;
	memset(&head,0,sizeof(head));
	USE use;
	memset(&use,0,sizeof(use));
	
	head.type = 2;
	strcpy(use.username,"yuangong");
	strcpy(use.password,"123456");
	memcpy(buf,&head,sizeof(head));
	memcpy(buf+sizeof(head),&use,sizeof(use));
	Write(buf,sockfd);

	while(1)
	{	
		memset(&head,0,sizeof(head));
		puts("4.进货 5.出货 0.退出");
		scanf("%d",&head.type);
		switch(head.type)
		{
			case 4:
				Input_Foot(&use,&head,sockfd);break;
			case 5:
				Output_Foot(&use,&head,sockfd);break;
			case 0:	break;
			default:
					puts("没有此功能选项");
				break;
		}
		usleep(100000);
		if (head.type==0)
			break;
	}
}
