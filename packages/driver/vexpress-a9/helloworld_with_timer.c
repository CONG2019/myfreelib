#include <linux/module.h>
#include <linux/time.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/ktime.h>

unsigned long start_time;

static int __init first_init(void)
{
    //ktime_get_boottime_ts64(&start_time);
    //getnstimeofday(&start_time);
    start_time = jiffies_to_msecs(jiffies) / 1000;
    pr_info("Load first start_time6=%lu.\n", start_time);
    return 0;
}

static void __exit first_exit(void){
    unsigned long end_time;
    //struct timespec64 end_time;
    //ktime_get_boottime_ts64(&end_time);
    //getnstimeofday(&end_time);
    end_time = jiffies_to_msecs(jiffies) / 1000;
    pr_info("unload module after %lu seconds, end_time=%lu\n", end_time-start_time, end_time);
    //pr_info("unload module after %lu seconds, end_time=%lu\n", end_time.tv_sec-start_time.tv_sec, end_time.tv_sec);
}

module_init(first_init);
module_exit(first_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Jacken vip.jacken2@gmail.com");
MODULE_DESCRIPTION("This is a module that will print the time since it was load");
