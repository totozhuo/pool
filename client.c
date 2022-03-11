/*************************************************************************
    > File Name: client.c
# File Name: client.c
# Author : Mayanrong  
# QQ : 1684615293
# Email:1684615293@qq.com
# Created Time: 2022年03月01日 星期二 10时51分34秒
# 描述：                   
 ************************************************************************/

#include"head.h"
#include"function.h"

MYSQL mysql;
sem_t sem;

int main()
{
	int sockfd=0,ret=0,len=0;
	char buf[1024]={0};
	HEAD head;USE use = {0};
	memset(&head,0,sizeof(head));

	struct sockaddr_in addr;
	addr.sin_family=AF_INET;
	addr.sin_port=10086;
	addr.sin_addr.s_addr=inet_addr("127.0.0.1");
	len=sizeof(addr);

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
		int a=Read(sockfd);
		if(a==-1)
			break;	
		else if (a==0)
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
						Read(sockfd);
						break;
					case 4:
						Input_Foot(&use,&head,sockfd);
						Read(sockfd);
						break;
					case 5:
						Output_Foot(&use,&head,sockfd);
						Read(sockfd);
						break;
					case 6:
						Clear_Food(&use,&head,sockfd);
     						Read(sockfd);
    						break;
					case 7:
					case 8:
					case 9:
					case 10:
					case 0:
						a=-1;	
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

