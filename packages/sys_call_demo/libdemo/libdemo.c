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

    lib_handler = dlopen(COLORPRINT_LIB_PATH, RTLD_LAZY);
}
