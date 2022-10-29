#include<linux/init.h>
#include<linux/kernel.h>
#include<linux/module.h>
#include <linux/ktime.h>
MODULE_LICENSE("Dual BSD/GPL");

void GetTime(int *phour, int *pmin, int *psec)
{
    long long ts;
    long long secs_per_day = 60*60*24;

    ts = ktime_get_real();
    /*convert from ns to s: */
    ts = ts/1000000000;
    
    /*Alternatively, we can get the time in seconds immediately using:
    ts = ktime_get_real_seconds();*/
    ts = ts%secs_per_day;

    *phour = ts/3600;
    *pmin  = (ts-*phour*3600)/60;
    *psec  = (ts-*phour*3600-*pmin*60);

    /* subtract 4 hours to get eastern time (works only in summer) */
    *phour-=4;
    if(*phour<0) *phour=*phour+24;
    
}

static int hello_init(void)
{
    int hour, min, sec;
    GetTime(&hour, &min, &sec);
    
    printk(KERN_ALERT"Hello,World\n");
    printk(KERN_ALERT"Current time is %02d:%02d:%02d\n", hour, min, sec);

    return 0;
}

static void hello_exit(void)
{
    int hour, min, sec;
    GetTime(&hour, &min, &sec);
   
    printk(KERN_ALERT"Goodbye,cruel world\n");
    printk(KERN_ALERT"Current time is %02d:%02d:%02d\n", hour, min, sec);
}

module_init(hello_init);
module_exit(hello_exit);

