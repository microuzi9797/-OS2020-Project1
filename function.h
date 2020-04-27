#include <sys/types.h>

#define MAIN_CPU 0
#define PROCESS_CPU 1
#define GET_TIME 333
#define SHOW_TIME 334

typedef struct process
{
	char Name[32];
	int R;
	int T;
	pid_t pid;
}Process;

static int current_time;
static int running_index;
static int finished_num;
static int last_time;

void unit_time();
void scheduling(Process *proc,int N,int policy);
void assign_CPU(pid_t pid,int core);
void wake_up_process(pid_t pid);
pid_t create_process(Process proc);
void block_process(pid_t pid);
int pick_next_process(Process *proc,int N,int policy);
int pick_by_FIFO(Process *proc,int N);
int pick_by_RR(Process *proc,int N);
int pick_by_SJF_or_PSJF(Process *proc,int N);