/***************************************************
> Copyright (C) 2022 ==KINGYI== All rights reserved.
> File Name: condition.c
> Author: LV
> Mail:joseph2009@163.com 
> Created Time: 2022年01月10日 星期一 19时54分55秒
***************************************************/

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <semaphore.h>

int quen[5] = {0};
sem_t cost,product;

//puduct
void* _puduct(void*arg)
{
	int i = 0;
	while(1)
	{
		sem_wait(&cost);        
		quen[i] += 1;
		printf("%d位-%d\n",i,quen[i]);
		sleep(1);
		sem_post(&product);     
		
		i = (i+1)%5;
	}
	pthread_exit(NULL);
}

//custom
void * _custom(void*arg)
{
	int i = 0;
	while(1)
	{
		sem_wait(&product);        
		quen[i] -= 1;
		printf("\t%d位-%d\n",i,quen[i]);
		sleep(1);
		sem_post(&cost);    
		
		i = (i+1)%5;
	}
	pthread_exit(NULL);
}

int main(int argc,const char* argv[])
{
	int i = 0;
	pthread_t pid[4];
	pthread_t tid[4];
	//time
	srand(time(NULL));

	sem_init(&cost,0,4);     
	sem_init(&product,0,0);  

	for(i = 0;i < 4;i++)
	{
		pthread_create(&pid[i],NULL,_puduct,NULL);
		pthread_create(&tid[i],NULL,_custom,NULL);
	}
	
	for(i = 0;i < 4;i++)
	{
		pthread_join(pid[i],NULL);
		pthread_join(tid[i],NULL);
	}

	sem_destroy(&cost);
	sem_destroy(&product);

    return 0;
}
