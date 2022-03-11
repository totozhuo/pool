/*************************************************************************
    > File Name: other_function.c
# File Name: other_function.c
# Author : Mayanrong  
# QQ : 1684615293
# Email:1684615293@qq.com
# Created Time: 2022年03月10日 星期四 14时06分47秒
# 描述：                   
 ************************************************************************/
 
#include"head.h"
#include"function.h"
	
//获取当地时间
char* Time()
{
	time_t t=time(NULL);
	struct tm *p=localtime(&t);
	static char buf[100]={0};
	sprintf(buf,"%4d-%2d-%2d-%02d:%02d:%02d",p->tm_year+1900,p->tm_mon+1,
			p->tm_mday,p->tm_hour,p->tm_min,p->tm_sec);

	return buf;
}

//错误日志
void Slip(char *p)
{
	int fd=0,ret=0;
	char str[1024]={0};

//错误信息采集
	strcpy(str,p);
	strcat(str,"错误原因：\t");
	strcat(str,strerror(errno));
	strcat(str,"发生时间：\t");
	strcat(str,Time());
	strcat(str,"\n");

//打开错误日志文件
	fd=open("./error.txt",O_RDWR|O_CREAT|O_APPEND,0644);
	if (fd<0)
	{

		perror("open");
		exit(-1);
	}

	ret=write(fd,str,strlen(str));
	if (ret<0)
	{
		perror("write");
		exit(-1);
	}
	close(fd);
}

//队列插入
void push_queue(struct node *queue,int data)
{
	struct node *p = queue;
	struct node *q = (struct node *)malloc(sizeof(struct node));
	memset(q,0,sizeof(struct node));
	q->data = data;

	while((p->next) != NULL)
	{
		p = p->next;
	}
	p->next = q;
	q->next = NULL;
}
//删除队列元素
int pop_queue(struct node *queue)
{
	int data = 0;
	struct node *p = NULL;
	p = queue->next;
	data = p->data;
	queue->next = p->next;
	free(p);

	return data;
}

//判断队列是否为空
int is_empty(struct node *queue)
{
	struct node *p = queue->next;
	if(p == NULL)
		return 1;
	else
		return 0;
}

