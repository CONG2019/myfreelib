#include <linux/module.h>

static int num = 5;
module_param(num, int, S_IRUGO);

static int __init hello_init(void){
    pr_info("helloworld init, num=%d\n", num);
    return 0;
}

static void __exit  hello_exit(void){
    pr_info("helloworld exit, num=%d\n", num);
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("jiancong_zhang 1178187720@qq.com");
MODULE_DESCRIPTION("This is a print out hello world module");
