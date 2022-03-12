/*************************************************************************
> File Name: function.h
# File Name: function.h
# Author : 李卓，苟涛，马彦荣 
# QQ : 1684615293
# Email:1684615293@qq.com
# Created Time: 2022年03月01日 星期二 11时40分25秒
# 描述：                   
 ************************************************************************/

#ifndef _FUNCTION_H_
#define _FUNCTION_H_

typedef struct 
{
	int fd;
	char data[BUFSIZ];
}DT;

typedef struct 
{
	int type;	
}HEAD;

typedef struct 
{
	char username[30];
	char password[30];
	int batch;	//批次
	char name[30];
	int all_count; //总量
	int put_count;//出货量
	int remain_count;//余量
	int purchase_time;//进货时间
	int shipping_time;//出货时间
	int expiration_day;	//保质期
	int place;	//位置
}USE;

typedef struct
{
	char name[20];
	char type[520];
	
}OP;

//lizhuo function
void initDB();
void RunIMServer();
void StopIMServer();
void* recv_work(void* arg);
void* do_work(void* arg);
void RegisterFunc(USE *use ,DT *node);
void* write_work(void* arg);

//错误日志头文件
char* Time();
void Slip(char *p);
void Oper();
//数据库
void Log_In_Find(USE *use,int accfd);

//登录注册
void Sign_In(HEAD *p,int accfd);
void Register(HEAD *p,int accfd);

//服务器
int Read(int sockfd);
void Write(char *buf,int sockfd);
void Read_find(int sockfd);
void * Rd(void*arg);
//仓库操作
void AddNemfood(USE *use,int accfd);
void Addfood(USE *use,int accfd);
void Input_Newfoot(USE *use, HEAD *head,int accfd);
void Input_Foot(USE *use, HEAD *head,int accfd);
void Output_Foot(USE *use, HEAD *head,int accfd);
int  Outfood(USE *use,int accfd);
void ClearFood(USE *use,int accfd);
void Clear_Food(USE *use, HEAD *head,int accfd);
void Findfood(USE *use,int accfd);
void Find_Food(USE *use, HEAD *head,int accfd);
void Smart_Infood(USE *use, HEAD *head,int accfd);
void SmartInfood(USE *use,int accfd);
int  Get_remarn(USE *use);
void Allot_food(USE *use, HEAD *head,int accfd);
void Allotfood(USE *use,int accfd);
int  Outfoodnor(USE *use,int accfd);
void  Addfoodnor(USE *use,int accfd);

#endif

