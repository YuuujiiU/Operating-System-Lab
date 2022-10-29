#include<linux/init.h>
#include<linux/kernel.h>
#include <linux/miscdevice.h>
#include<linux/module.h>
#include <linux/ktime.h>
#include<linux/timekeeping.h>
#include <linux/uaccess.h>
#include <linux/fs.h>

MODULE_LICENSE("Dual BSD/GPL");

static char message[100];

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


static int misc_open(struct inode* inode, struct file* filp){

    int hour, min, sec;
    GetTime(&hour, &min, &sec);

    //printk(KERN_ALERT"Current time is %02d:%02d:%02d\n", hour, min, sec);
    sprintf(message, "Hello world, current time is %02d:%02d:%02d\n", hour, min, sec);
	return 0;
}

static ssize_t misc_read(struct file* filp, char* buffer, size_t length, loff_t* ppos){
	int max_bytes;
	int bytes_to_read;
	
	// If *ppos equals to the size of the message, stop reading copying to buffer.
	if (*ppos == strlen(message)+1){
		return 0;
	}
	
	max_bytes = (strlen(message)+1) - *ppos;
	
	if (max_bytes < length){
		bytes_to_read = max_bytes;
	} else{
		bytes_to_read = length;
	}
	
	copy_to_user(buffer, message + *ppos, bytes_to_read);	// Send the message to user.
	*ppos += bytes_to_read;
	
	return bytes_to_read;
}

static int misc_release(struct inode* inode, struct file* filp){
	return 0;
}

static const struct file_operations fops = {
    .read = misc_read,
    .open = misc_open,
    .release = misc_release
};

static struct miscdevice misc = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "lab4",
	.fops = &fops,
};

int misc_init(void){
	int register_return;
        
	if ((register_return = misc_register(&misc))){
		printk(KERN_ERR "Failed to register the device\n");
		return register_return;
	}
	return 0;
}

void misc_exit(void){
	misc_deregister(&misc);
}

module_init(misc_init)
module_exit(misc_exit)

