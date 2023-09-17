#include <linux/module.h>
#include <linux/cdev.h> 
#include <linux/fs.h>
#include <linux/miscdevice.h>

static int my_dev_open(struct inode *inode, struct file *file){
    pr_info("%s is called.\n", __func__);
    return 0;
}

static int my_dev_close(struct inode *inode, struct file *file){
    pr_info("%s is called.\n", __func__);
    return 0;
}

static long my_dev_ioctl(struct file *file, unsigned int cmd, unsigned long arg){
    pr_info("%s is called. cmd=%d, arg=%ld\n", __func__, cmd, arg);
    return 0;
}

static const struct file_operations my_dev_fops = {
    .owner = THIS_MODULE,
    .open = my_dev_open,
    .release = my_dev_close,
    .unlocked_ioctl = my_dev_ioctl,
};

static struct miscdevice helloworld_miscdevice = {
    .minor = MISC_DYNAMIC_MINOR,
    .name = "mydev",
    .fops = &my_dev_fops,
};

static int __init my_driver_init(void){
    int ret;

    pr_info("my driver init.\n");

    ret = misc_register(&helloworld_miscdevice);
    if(ret < 0){
        pr_info("Unable to allocate misc minor number\n");
        return ret;
    }

    pr_info("mydev: got minor %i\n", helloworld_miscdevice.minor);

    return 0;
}

static void __exit my_driver_exit(void){
    pr_info("my driver exit.\n");
    misc_deregister(&helloworld_miscdevice);
}

module_init(my_driver_init);
module_exit(my_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Jacken vip.jacken2@gmail.com");
MODULE_DESCRIPTION("Tist is a module that test the misc char device driver");