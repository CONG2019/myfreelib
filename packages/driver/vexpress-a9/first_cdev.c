#include <linux/module.h>
#include <linux/cdev.h> 
#include <linux/fs.h>

#define MY_MAJOR_NUM 202

static struct cdev my_dev;

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

static int __init my_driver_init(void){
    int ret;

    dev_t dev = MKDEV(MY_MAJOR_NUM, 0);
    pr_info("my driver init.\n");

    ret = register_chrdev_region(dev, 1, "my_char_device");
    if(ret < 0){
        pr_info("Unable to allocate mayor number %d\n", MY_MAJOR_NUM);
        return ret;
    }

    cdev_init(&my_dev, &my_dev_fops);
    ret = cdev_add(&my_dev, dev, 1);
    if (ret < 0){
        unregister_chrdev_region(dev, 1);
        pr_info("Unable to add cdev\n");
        return  ret;
    }

    return 0;
}

static void __exit my_driver_exit(void){
    pr_info("my driver exit.\n");
    cdev_del(&my_dev);
    unregister_chrdev_region(MKDEV(MY_MAJOR_NUM,0), 1);
}

module_init(my_driver_init);
module_exit(my_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Jacken vip.jacken2@gmail.com");
MODULE_DESCRIPTION("Tist is a module that test the char device driver");