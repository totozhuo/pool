/*************************************************************************
    > File Name: client.c
# File Name: client.c
# Author : Mayanrong  
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
	int	ret = 0;
	socklen_t len = 0;
	char buf[1024] = {0};
	HEAD head;
	USE use = {0};
	memset(&head,0,sizeof(head));
	
	struct sockaddr_in addr;
	bzero(&addr,sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(atoi("10085"));
	addr.sin_addr.s_addr = inet_addr("127.1");
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
	/*	
	//创建心跳包线程
	pthread_t tid = 0;
	pthread_create(&tid,NULL,heart_work,(void*)&sockfd);//分离后不使用tid
	pthread_detach(tid);
	*/
	while(1)
	{
		memset(buf,0,sizeof(buf));
		memset(&head,0,sizeof(head));
		puts("1.注册 2.登录  0.退出");
		scanf("%d",&head.type);
		switch(head.type)
		{
			case 1:
				Sign_In(&head,sockfd);
				break;
			case 2:
				Register(&head,sockfd);
				break;
			case 0:
				memcpy(buf,&head,sizeof(HEAD));
				Write(buf,sockfd);
				break;
			default:
				puts("没有此功能选项");
				break;
		}
		
		int a = Read(sockfd);
		if(a != 0)
			break;	
		else if (a == 0)
		{
			while(1)
			{
				puts("3.新产品录入 4.进仓 5.出仓 6.旧产品下线 7.产品信息变更 8.查询 0.返回上一级");
				memset(&head,0,sizeof(head));
				scanf("%d",&head.type);
				switch(head.type)
				{
					case 3:
						Input_Newfoot(&use,&head,sockfd);
						break;
					case 4:
						Input_Foot(&use,&head,sockfd);
						break;
					case 5:
						Output_Foot(&use,&head,sockfd);
						break;
					case 6:
						Clear_Food(&use,&head,sockfd);
    					break;
					case 7:
					case 8:
					case 9:
					case 10:
					case 0:
						a = -1;	
						break;

					default:
						puts("暂时不支持其他功能，请重新选择");
						break;
				}
				
				if(a==-1)
					break;	
			}
		}	
	}

	close(sockfd);
	
	return 0;
}

