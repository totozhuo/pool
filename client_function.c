/*************************************************************************
    > File Name: client_function.c
# File Name: client_function.c
# Author : Mayanrong  
# QQ : 1684615293
# Email:1684615293@qq.com
# Created Time: 2022年03月10日 星期四 09时36分03秒
# 描述：                   
 ************************************************************************/


#include"head.h"
#include"function.h"
	
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
	printf("请输入该商品位置：\n");
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
