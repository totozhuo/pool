/*************************************************************************
    > File Name: function.h
# File Name: function.h
# Author : Mayanrong  
# QQ : 1684615293
# Email:1684615293@qq.com
# Created Time: 2022年03月01日 星期二 11时40分25秒
# 描述：                   
 ************************************************************************/

#ifndef _FUNCTION_H_
#define _FUNCTION_H_

typedef struct 
{
	char username[30];
	char password[30];
	int batch;	//批次
	char name[30];
	int count;
	int time1;
	int time2;
	int day;	//保质期
	int place;	//位置
	int putgoods;  //出货
}USE;

typedef struct 
{
	int type;	
}HEAD;

//队列头文件
struct node
{
	int data;
	struct node *next;
};

void push_queue(struct node *queue,int data);
int pop_queue(struct node *queue);
int is_empty(struct node *queue);

//错误日志头文件
char* Time();
void Slip(char *p);

//数据库
void Line();
void Inster(USE *use,int accfd);
void Log_In_Find(USE *use,int accfd);


//登录注册
void Sign_In(HEAD *p,int accfd);
void Register(HEAD *p,int accfd);

//服务器
void Write(char *buf,int sockfd);
int Read(int sockfd);
void *thread(void *arg);
int socket_init_sockfd(int sockfd);

//仓库操作
void  AddNemfood(USE *use,int accfd);
void  Addfood(USE *use,int accfd);
void Input_Newfoot(USE *use, HEAD *head,int accfd);
void Input_Foot(USE *use, HEAD *head,int accfd);
void Output_Foot(USE *use, HEAD *head,int accfd);
#endif
