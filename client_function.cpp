/*************************************************************************
    > File Name: client_function.c
# File Name: client_function.c
# Author : 李卓，苟涛，马彦荣  
# QQ : 1684615293
# Email:1684615293@qq.com
# Created Time: 2022年03月10日 星期四 09时36分03秒
# 描述：                   
 ************************************************************************/

#include "head.h"
#include "function.h"
	
using namespace std;

// 读取查找信息
void Read_find(int sockfd)
{
  int ret=0;
  char buf[1024]={0};
  ret=read(sockfd,buf,1023);
  if(ret<0)
  {
   memset(buf,0,sizeof(buf));
   sprintf(buf,"文件名：%s \t 函数名：%s \t 行号：%d\t",__FILE__,
     __FUNCTION__,__LINE__);
   Slip(buf);
   exit(1); 
  }
  USE* use = (USE*)buf;
  printf("货物位置：%d\t总进货量：%d\t总出货量：%d\t仓库剩余量：%d\n",use->place,use->all_count,use->put_count,use->remain_count);
}
//注册
void Sign_In(HEAD *p,int accfd)
{
	char buf[BUFSIZ]={0};
	HEAD head;//选择功能
	memset(&head,0,sizeof(head));
	
	USE use; //账号密码
	memset(&use,0,sizeof(use));
	head.type=p->type;
	
	puts("请输入需要注册的用户名,大于三位数");
	while(1)
	{
		scanf("%s",use.username);
		if(strlen(use.username)>2)
			break;
		else
			puts("用户名过于简单，请重新输入");
	}		
		
	puts("请设置密码，密码必须大于6位");
	while(1)
	{
		scanf("%s",use.password);
		if(strlen(use.password)>5)
			break;
		else
			puts("密码过于简单，请重新输入");		
	}	
	memcpy(buf,&head,sizeof(head));
	memcpy(buf+sizeof(head),&use,sizeof(use));
	Write(buf,accfd);
}

//登录
void Register(HEAD *p,int accfd)
{
	char buf[1024]={0};
	HEAD head;//选择功能
	memset(&head,0,sizeof(head));
	
	USE use; //账号密码
	memset(&use,0,sizeof(use));

	head.type=p->type;
	puts("请输入用户名");
	scanf("%s",use.username);
	puts("请输入密码");
	scanf("%s",use.password);
	memcpy(buf,&head,sizeof(head));
	memcpy(buf+sizeof(head),&use,sizeof(use));
	Write(buf,accfd);
}

void Input_Newfoot(USE *use, HEAD *head,int accfd)
{
	char buf[1024] = {0};
	printf("请输入品类名称：\n");
	scanf("%s",use->name);
	use->remain_count = 0;
	printf("请输入该商品保质期：\n");
	scanf("%d",&use->expiration_day);
	use->put_count = 0;
	use->remain_count=0;
	memcpy(buf,head,sizeof(HEAD));
	memcpy(buf+sizeof(HEAD),use,sizeof(USE));
	Write(buf,accfd);
}
//进仓
void Input_Foot(USE *use, HEAD *head,int accfd)
{
	char buf[1024] = {0};
	printf("请输入品类名称：\n");
	scanf("%s",use->name);
	printf("请输入商品数量：\n");
	scanf("%d",&use->all_count);
	memcpy(buf,head,sizeof(HEAD));
	memcpy(buf+sizeof(HEAD),use,sizeof(USE));
	Write(buf,accfd);
}
//出仓客户端
void Output_Foot(USE *use, HEAD *head,int accfd)
{
	char buf[1024] = {0};
	printf("请输入出货品类名称：\n");
	
	scanf("%s",use->name);
	printf("请输入出货商品数量：\n");
	scanf("%d",&use->put_count);
	memcpy(buf,head,sizeof(HEAD));
	memcpy(buf+sizeof(HEAD),use,sizeof(USE));
	Write(buf,accfd);
}

//旧产品下线客户端
void Clear_Food(USE *use, HEAD *head,int accfd)
{
 char buf[1024] = {0};
 printf("请输入下线货物名称：\n");
 scanf("%s",use->name);
 memcpy(buf,head,sizeof(HEAD));
 memcpy(buf+sizeof(HEAD),use,sizeof(USE));
 Write(buf,accfd);
}

//查询客户端
void Find_Food(USE *use, HEAD *head,int accfd)
{
 char buf[1024] = {0};
 printf("请输入查询货物名称：\n");
 scanf("%s",use->name);
 memcpy(buf,head,sizeof(HEAD));
 memcpy(buf+sizeof(HEAD),use,sizeof(USE));
 Write(buf,accfd);
}

//智能进货客户端
void Smart_Infood(USE *use, HEAD *head,int accfd)
{
	char buf[1024] = {0};
	printf("请输入智能进货名称：\n");
	scanf("%s",use->name);
	memcpy(buf,head,sizeof(HEAD));
	memcpy(buf+sizeof(HEAD),use,sizeof(USE));
	Write(buf,accfd);
}
//调拨客户端
void Allot_food(USE *use, HEAD *head,int accfd)
{
	char buf[1024] = {0};
	printf("请输入需要调拨货物名称：\n");
	scanf("%s",use->name);
	printf("请输入需要调拨货物数量：\n");
	scanf("%d",&use->put_count);
	printf("请输入出仓名称：\n");
	scanf("%d",&use->all_count);
	printf("请输入进仓名称：\n");
	scanf("%d",&use->remain_count);
	memcpy(buf,head,sizeof(HEAD));
	memcpy(buf+sizeof(HEAD),use,sizeof(USE));
	Write(buf,accfd);
}

