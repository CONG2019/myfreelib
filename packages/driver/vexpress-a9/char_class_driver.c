#include <linux/module.h>
#include <linux/cdev.h> 
#include <linux/fs.h>
#include <linux/device.h>

#define DEVICE_NAME "mydev"
#define CLASS_NAME  "hello_class"

static struct class *helloClass;
static struct cdev my_dev;
static struct cdev my_dev2;
static int major;
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
    dev_t dev_no;
    struct device *helloDevice;
    struct device *helloDevice2;
    pr_info("my driver init.\n");

    ret = alloc_chrdev_region(&dev_no, 0, 1, DEVICE_NAME);   
    if(ret < 0){
        pr_info("Unable to allocate mayor number\n");
        return ret;
    }

    major = MAJOR(dev_no);
    dev = MKDEV(major,0);
    pr_info("correctly major number %d\n", major);

    cdev_init(&my_dev, &my_dev_fops);
    ret = cdev_add(&my_dev, dev, 1);
    cdev_init(&my_dev2, &my_dev_fops);
    ret = cdev_add(&my_dev2, MKDEV(major,1), 1);
    if (ret < 0){
        unregister_chrdev_region(dev, 1);
        pr_info("Unable to add cdev\n");
        return  ret;
    }

    // regisetr the device class
    helloClass = class_create(THIS_MODULE, CLASS_NAME);
    if (IS_ERR(helloClass)){
        cdev_del(&my_dev);
        unregister_chrdev_region(dev, 1);
        pr_info("failed to register device class\n");
        return PTR_ERR(helloClass);
    }

    // create a device node
    helloDevice = device_create(helloClass, NULL, dev, NULL, DEVICE_NAME);
    helloDevice2 = device_create(helloClass, NULL, MKDEV(major,1), NULL, DEVICE_NAME"1");
    if(IS_ERR(helloDevice)){
        class_destroy(helloClass);
        cdev_del(&my_dev);
        unregister_chrdev_region(dev, 1);
        pr_info("failed to create the device\n");
        return PTR_ERR(helloDevice);
    }

    return 0;
}

static void __exit my_driver_exit(void){
    pr_info("my driver exit.\n");
    device_destroy(helloClass, dev);
    device_destroy(helloClass, MKDEV(major, 1));
    class_destroy(helloClass);
    cdev_del(&my_dev);
    cdev_del(&my_dev2);
    unregister_chrdev_region(dev, 1);
}

module_init(my_driver_init);
module_exit(my_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Jacken vip.jacken2@gmail.com");
MODULE_DESCRIPTION("Tist is a module that test the char device driver");