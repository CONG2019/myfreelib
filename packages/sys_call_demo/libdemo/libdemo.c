#include <stdio.h>
#include <stdlib.h>
#include "colorprint.h"
#include <dlfcn.h>  //need to add -ldl when compile

#define COLORPRINT_LIB_PATH "../../../usr/lib/libclprint.so"

int main(int argc, char const *argv[])
{
    /* code */
    return 0;
}

void dldemo(void){
    void *lib_handler;
    void (*clprogress_print)(const char *, ...);

    lib_handler = dlopen(COLORPRINT_LIB_PATH, RTLD_LAZY);
    if(!lib_handler) printf("Open %s failed!\n", COLORPRINT_LIB_PATH);

    clprogress_print = dlsym(lib_handler, "clprogress_print");
    if(!clprogress_print) printf("Can not find symbol clprogress_print.\n" );

    clprogress_print("Success.\n");

    dlclose(lib_handler);
}

int main(int argc, char const *argv[])
{
    dldemo();
    return 0;
}

