#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sched.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include "function.h"

void assign_CPU(pid_t pid,int core)
{
	cpu_set_t mask;
	CPU_ZERO(&mask);
	CPU_SET(core,&mask);
	if(sched_setaffinity(pid,sizeof(cpu_set_t),&mask) < 0)
	{
		printf("assign CPU error\n");
		exit(-1);
	}
}

void wake_up_process(pid_t pid)
{
	struct sched_param param;
	param.sched_priority = 0;
	sched_setscheduler(pid,SCHED_OTHER,&param);
}

pid_t create_process(Process proc)
{
	pid_t pid;
	//child process: new process
	if((pid = fork()) == 0)
	{
		assign_CPU(getpid(),PROCESS_CPU);
		unsigned long int start_sec,start_nsec;
		unsigned long int end_sec,end_nsec;
		char dmesg[200];
		syscall(GET_TIME,&start_sec,&start_nsec);
		for(int i = 0;i < proc.T;i++)
		{
			unit_time();
		}
		syscall(GET_TIME,&end_sec,&end_nsec);
		sprintf(dmesg,"[Project1] %d %lu.%09lu %lu.%09lu\n",getpid(),start_sec,start_nsec,end_sec,end_nsec);
		syscall(SHOW_TIME,dmesg);
		exit(0);
	}
	//parent process: main process
	else
	{
		return pid;
	}
}

void block_process(pid_t pid)
{
	struct sched_param param;
	param.sched_priority = 0;
	sched_setscheduler(pid,SCHED_IDLE,&param);
}