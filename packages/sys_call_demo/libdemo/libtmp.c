#include <stdio.h>
#include <stdlib.h>

#ifdef LIB_HIDDEN_TEST //Practise code for Dynamic
__attribute__ ((visibility("hidden"))) void lprint(void){
    printf("%s\n", __func__);
}
#endif

#ifdef LIB_HIDDEN_TEST
__attribute__ ((visibility("hidden"))) void y(void){
    printf("function: %s\n", __func__);
}
#else
void y(void){
    printf("function: %s\n", __func__);
}
#endif

void x(void){
    printf("function: %s\n", __func__);
    y();
}

__asm__(".symver xyz_old,xyz@VER_1");   //指示于版本VER_1进行链接的应用程序所使用的xyz的实现是xyz_old
__asm__(".symver xyz_new,xyz@@VER_2");	//指示应用程序与这个共享库进行静态链接时应使用xyz的默认定义只有一个指令使用@@
void xyz_old(){
    printf("%s\n", __func__);
}

void xyz_new(){
    printf("%s\n", __func__);
}