/*************************************************************************
> File Name: server.c
# File Name: server.c
# Author : Mayanrong  
# QQ : 1684615293
# Email:1684615293@qq.com
# Created Time: 2022年03月01日 星期二 11时59分25秒
# 描述：                   
 ************************************************************************/

#include "head.h"
#include "function.h"

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
