/***************************************************
> Copyright (C) 2021 ==KINGYI== All rights reserved.
> File Name: 04大写转小写.c
> Author: Kingyi
> Mail:joseph2009@163.com 
> Created Time: 2021年11月09日 星期二 11时30分13秒
***************************************************/

#include <stdio.h>
char toLower(char ch)
{
	return ch-'A'+'a';
}

int main(int argc,const char* argv[])
{
	char ch = 0;

	printf("请输入一个字母：\n");
	scanf("%c",&ch);

	printf("%c\n",toLower(ch));
    return 0;
}
