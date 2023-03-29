#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/kobject.h>
#include <linux/sysfs.h>

static struct kobject *my_kobj;

static ssize_t my_sysfs_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
    return sprintf(buf, "my_sysfs_attr\n");
}

static struct kobj_attribute my_sysfs_attr = __ATTR(my_sysfs_attr, 0664, my_sysfs_show, NULL);

static int __init my_module_init(void)
{
    int ret;

    // Create a new kobject
    my_kobj = kobject_create_and_add("my_kobject_hello", kernel_kobj);
    if (!my_kobj)
        return -ENOMEM;

    // Create a new sysfs attribute for the kobject
    ret = sysfs_create_file(my_kobj, &my_sysfs_attr.attr);
    if (ret) {
        kobject_put(my_kobj);
        return ret;
    }

    // Send a hotplug event with the message "my_module loaded"
    kobject_uevent_env(my_kobj, KOBJ_ADD, (char *[]){"HOTPLUG", "ACTION=add", "DEVPATH=/my_kobject", "MESSAGE=my_module loaded", NULL});

    return 0;
}

static void __exit my_module_exit(void)
{
    // Send a hotplug event with the message "my_module unloaded"
    kobject_uevent_env(my_kobj, KOBJ_REMOVE, (char *[]){"HOTPLUG", "ACTION=remove", "DEVPATH=/my_kobject", "MESSAGE=my_module unloaded", NULL});

    // Remove the sysfs attribute and kobject
    sysfs_remove_file(my_kobj, &my_sysfs_attr.attr);
    kobject_put(my_kobj);

    pr_info("my_module unloaded\n");
}

module_init(my_module_init);
module_exit(my_module_exit);


MODULE_LICENSE("GPL");
MODULE_AUTHOR("jiancong_zhang 1178187720@qq.com");
MODULE_DESCRIPTION("This is a module that will send uevent when insmod and rmmod.");
