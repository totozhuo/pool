/***************************************************
  > Copyright (C) 2022 ==KINGYI== All rights reserved.
  > File Name: udp_server.c
  > Author: LV
  > Mail:joseph2009@163.com 
  > Created Time: 2022年01月13日 星期四 23时20分10秒
 ***************************************************/

#include "hand.h"

using namespace std;
deque<DT> P;
MYSQL mysql;  
int iserver = 0;

int main(int argc,const char* argv[])
{
	initDB();
	while(1)
	{
		printf("请输入命令:>\n");
		char command[255];
		scanf("%s",command);

		if(strcmp(command,"run")==0)
		{
			RunIMServer();
		}
		else if(strcmp(command,"stop")==0)
		{
			StopIMServer();
		}
		else if(strcmp(command,"exit")==0)
		{
			printf("bye----bye\n");
			exit(0);
		}
		else
		{
			printf("对不起,该命令不识别!!!\n");
		}
	}

	mysql_close(&mysql);

	return 0;
}
