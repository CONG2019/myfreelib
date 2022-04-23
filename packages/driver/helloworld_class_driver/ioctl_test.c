#include <stdio.h> 
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>

// mknod /dev/mydev c 222 0
int main(void){
    int my_dev = open("/dev/hello_dev", 0);

    if(my_dev < 0){
        printf("open mydev error.\n");
    }else {
        ioctl(my_dev, 100, 110);
        close(my_dev);
    }

    return 0;
}