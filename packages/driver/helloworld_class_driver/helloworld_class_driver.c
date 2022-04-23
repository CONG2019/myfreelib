#include <linux/module.h>
#include <linux/device.h>
#include <linux/cdev.h> 
#include <linux/fs.h>

// #define MY_MAJOR_NUM 222
#define DEVICE_NAME "hello_dev"
#define CLASS_NAME  "hello_class"

static struct class* helloClass;
static struct device* helloDevice;
static struct cdev my_dev;
dev_t dev;

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
    dev_t dev_no;
    int Major;

    // dev_t dev = MKDEV(MY_MAJOR_NUM, 0);
    pr_info("helloworld init.\n");

    ret = alloc_chrdev_region(&dev_no, 0, 1, DEVICE_NAME);
    // ret = register_chrdev_region(dev, 1, "my_char_driver");
    if(ret < 0){
        pr_info("alloc_chrdev_region failed.\n");
        return ret;
    }
    Major = MAJOR(dev_no);
    dev = MKDEV(Major,0);
    pr_info("Major=%d, dev_no=%d, dev=%d\n", Major, dev_no, dev);

    cdev_init(&my_dev, &my_dev_fops);
    ret = cdev_add(&my_dev, dev, 1);
    if(ret < 0){
        unregister_chrdev_region(dev, 1);
        pr_info("cdev_add failed.\n");
        return ret;
    }

    helloClass = class_create(THIS_MODULE, CLASS_NAME);
    if(IS_ERR(helloClass)){
        unregister_chrdev_region(dev, 1);
        cdev_del(&my_dev);
        pr_info("class_create failed.\n");
        return PTR_ERR(helloClass);
    }
    pr_info("class_create success\n");

    // create a device node 
    helloDevice = device_create(helloClass, NULL, dev, NULL, DEVICE_NAME);
    if(IS_ERR(helloDevice)){
        class_destroy(helloClass);
        unregister_chrdev_region(dev, 1);
        cdev_del(&my_dev);
        pr_info("device_create failed.\n");
        return PTR_ERR(helloDevice);
    }
    pr_info("create a device node success.\n");

    return 0;
}

static void __exit hello_exit(void){
    pr_info("helloworld exit\n");
    device_destroy(helloClass, dev);
    class_destroy(helloClass);
    cdev_del(&my_dev);
    unregister_chrdev_region(dev, 1);
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("jiancong_zhang 1178187720@qq.com");
MODULE_DESCRIPTION("This is a helloworld char driver");