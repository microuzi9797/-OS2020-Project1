#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "function.h"

int compare(const void *data1, const void *data2);

void scheduling(Process *proc, int N, int policy)
{
	// separate main process and new process' CPUs
	assign_CPU(getpid(), MAIN_CPU);
	wake_up_process(getpid());
	// initialize
	current_time = 0;
	running_index = -1;
	finished_num = 0;
	for(int i = 0;i < N;i++)
	{
		proc[i].pid = -1;
	}
	// sort by ready time
	qsort(proc, N, sizeof(Process), compare);

	while(1)
	{
		// the running process finished or not
		if(running_index != -1 && proc[running_index].T == 0)
		{
			waitpid(proc[running_index].pid, NULL, 0);
			printf("%s %d\n", proc[running_index].Name, proc[running_index].pid);
			running_index = -1;
			finished_num++;
			if(finished_num == N)
			{
				break;
			}
		}
		// fork to create the process
		for(int i = 0;i < N;i++)
		{
			if(proc[i].R == current_time)
			{
				proc[i].pid = create_process(proc[i]);
				block_process(proc[i].pid);
			}
		}
		// pick next process
		int next_index;
		next_index = pick_next_process(proc, N, policy);
		if(next_index != -1)
		{
			// context switch
			if(running_index != next_index)
			{
				wake_up_process(proc[next_index].pid);
				block_process(proc[running_index].pid);
				running_index = next_index;
				last_time = current_time;
			}
		}
		// time++
		unit_time();
		if(running_index != -1)
		{
			proc[running_index].T--;
		}
		current_time++;
	}
}

int compare(const void *data1, const void *data2)
{
	Process *ptr1 = (Process *)data1;
	Process *ptr2 = (Process *)data2;
	if(ptr1->R < ptr2->R)
	{
		return -1;
	}
	else if(ptr1->R > ptr2->R)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

int pick_next_process(Process *proc, int N, int policy)
{
	// non-preemptive
	if(running_index != -1 && (policy == 1 || policy == 3))
	{
		return running_index;
	}
	int pick;
	// policy: FIFO
	if(policy == 1)
	{
		pick = pick_by_FIFO(proc, N);
	}
	// policy: RR
	else if(policy == 2)
	{
		pick = pick_by_RR(proc, N);
	}
	// policy: SJF or PSJF
	else if(policy == 3 || policy == 4)
	{
		pick = pick_by_SJF_or_PSJF(proc, N);
	}
	return pick;
}

int pick_by_FIFO(Process *proc, int N)
{
	int pick = -1;
	for(int i = 0;i < N;i++)
	{
		if(proc[i].pid == -1 || proc[i].T == 0)
		{
			continue;
		}
		else if(pick == -1 || proc[i].R < proc[pick].R)
		{
			pick = i;
		}
	}
	return pick;
}

int pick_by_RR(Process *proc, int N)
{
	int pick = -1;
	if(running_index == -1)
	{
		for(int i = 0;i < N;i++)
		{
			if(proc[i].pid != -1 && proc[i].T > 0)
			{
				pick = i;
				break;
			}
		}
	}
	else if((current_time - last_time) % 500 == 0)
	{
		pick = (running_index + 1) % N;
		while(proc[pick].pid == -1 || proc[pick].T == 0)
		{
			pick = (pick + 1) % N;
		}
	}
	else
	{
		pick = running_index;
	}
	return pick;
}

int pick_by_SJF_or_PSJF(Process *proc, int N)
{
	int pick = -1;
	for(int i = 0;i < N;i++)
	{
		if(proc[i].pid == -1 || proc[i].T == 0)
		{
			continue;
		}
		else if(pick == -1 || proc[i].T < proc[pick].T)
		{
			pick = i;
		}
	}
	return pick;
}