#include <stdio.h>
#include <string.h>
#include "function.h"

int main()
{
    //Read the input
    char S[5];
    scanf("%s",S);
    int N;
    scanf("%d",&N);
    Process proc[N];
    for(int i = 0;i < N;i++)
    {
        scanf("%s %d %d",proc[i].Name,&proc[i].R,&proc[i].T);
    }
    //assign the policy and schedule
    int policy;
    if(strcmp(S,"FIFO") == 0)
    {
        policy = 1;
	}
	else if(strcmp(S,"RR") == 0)
    {
        policy = 2;
	}
	else if(strcmp(S,"SJF") == 0)
    {
        policy = 3;
	}
	else if(strcmp(S,"PSJF") == 0)
    {
        policy = 4;
    }
    scheduling(proc,N,policy);
    return 0;
}

void unit_time()
{
	volatile unsigned long i;
	for(i = 0;i < 1000000UL;i++);
}