/***************************************************
> Copyright (C) 2022 ==KINGYI== All rights reserved.
> File Name: hand.h
> Author: LV
> Mail:joseph2009@163.com 
> Created Time: 2022年01月14日 星期五 22时48分25秒
***************************************************/

#ifndef _HAND_H_
#define _HAND_H_

#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <time.h>
#include <string.h>
#include <errno.h>
#include <mysql/mysql.h>
#include <deque>

typedef struct regist
{
	int id;
	char UserAccound[20];
	int PassWd;
}REGISTER;

typedef struct 
{
	int fd;
	char data[BUFSIZ];
}DT;

void initDB();
void RunIMServer();
void StopIMServer();
void* recv_work(void* arg);
void* do_work(void* arg);
void json_string(REGISTER *res,char *data);
void RegisterFunc(REGISTER *res ,DT *node);
void* write_work(void* arg);
void LoginFunc(REGISTER *res ,DT *node);

#endif
