/*************************************************************************
    > File Name: client.c
# File Name: client.c
# Author : 李卓，苟涛，马彦荣  
# QQ : 1684615293
# Email:1684615293@qq.com
# Created Time: 2022年03月01日 星期二 10时51分34秒
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
	int sockfd = 0;
	int	ret = 0,c = 0;
	socklen_t len = 0;
	char buf[1024] = {0};
	HEAD head;
	USE use = {0};
	memset(&head,0,sizeof(head));
	
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
	ret=connect(sockfd,(struct sockaddr*)&addr,len);
	if(ret<0)
	{
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

	while(1)
	{
		do
		{
			memset(&head,0,sizeof(head));
			puts("1.注册 2.登录  0.退出");
			while(1)
			{
				c=scanf("%d",&head.type);
				while(getchar()!='\n');
				if(c!=1)
					puts("请输入正确的数字");
				else
					break;
			}

			switch(head.type)
			{
				case 1:
					Sign_In(&head,sockfd);break;
				case 2:
					Register(&head,sockfd);break;
				case 0:
					exit(1);break;
				default:
					head.type = 520;
					memcpy(buf,&head,sizeof(HEAD));
					Write(buf,sockfd);break;
			}

		}while(Read(sockfd));

		do
		{
			puts("3.新产品录入 4.进仓 5.出仓 6.旧产品下线 7.查询 8.智能进货 9.货物调拨 0.返回上一级");
			memset(&head,0,sizeof(head));
			while(1)
			{
				c=scanf("%d",&head.type);
				while(getchar()!='\n');
				if(c!=1)
					puts("请输入正确的数字");
				else
					break;
			}

			switch(head.type)
			{
				case 3:
					Input_Newfoot(&use,&head,sockfd);break;
				case 4:
					Input_Foot(&use,&head,sockfd);
					Read(sockfd);break;
				case 5:
					Output_Foot(&use,&head,sockfd);
					Read(sockfd);break;
				case 6:
					Clear_Food(&use,&head,sockfd);break;
				case 7:
					Find_Food(&head,sockfd);
					Read_find(sockfd);break;
				case 8:
					Smart_Infood(&use,&head,sockfd);
					Read(sockfd);
					break;
				case 9:
					Allot_food(&use,&head,sockfd);
					Read(sockfd);break;
				case 0:break;
				default:
					puts("暂时不支持其他功能，请重新选择");
					break;
			}

		}while(head.type);		
	}
	close(sockfd);
	return 0;
}
