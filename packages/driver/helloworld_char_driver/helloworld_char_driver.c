#include <linux/module.h>

#include <linux/cdev.h> 
#include <linux/fs.h>

#define MY_MAJOR_NUM 222

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
    pr_info("%s is called. cmd = %d, arg = %ld\n", __func__, cmd, arg);
    return 0;
}

static const struct file_operations my_dev_fops = {
    .owner = THIS_MODULE,
    .open = my_dev_open,
    .release = my_dev_close,
    .unlocked_ioctl = my_dev_ioctl,
};

static int __init hello_init(void){
    int ret;

    dev_t dev = MKDEV(MY_MAJOR_NUM, 0);
    pr_info("helloworld init.\n");

    ret = register_chrdev_region(dev, 1, "my_char_driver");
    if(ret < 0){
        pr_info("register_chrdev_region failed.\n");
        return ret;
    }

    cdev_init(&my_dev, &my_dev_fops);
    ret = cdev_add(&my_dev, dev, 1);
    if(ret < 0){
        unregister_chrdev_region(dev, 1);
        pr_info("cdev_add failed.\n");
        return ret;
    }

    return 0;
}

static void __exit hello_exit(void){
    pr_info("helloworld exit\n");
    cdev_del(&my_dev);
    unregister_chrdev_region(MKDEV(MY_MAJOR_NUM, 0), 1);
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("jiancong_zhang 1178187720@qq.com");
MODULE_DESCRIPTION("This is a helloworld char driver");