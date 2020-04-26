#include <linux/kernel.h>
#include <linux/ktime.h>
#include <linux/timekeeping.h>
#include <linux/linkage.h>

asmlinkage int sys_get_time(unsigned long *sec, unsigned long *nsec)
{
    struct timespec t;
    getnstimeofday(&t);

    *sec = t.tv_sec;
    *nsec = t.tv_nsec;
    return 0;
}