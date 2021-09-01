#include <stdio.h>
#include <stdlib.h>
#include "colorprint.h"
#include <dlfcn.h>  //need to add -ldl when compile

#define COLORPRINT_LIB_PATH "/usr/lib/libclprint.so"

void dldemo(void){
    void *lib_handler;
    void (*clprogress_print)(const char *, ...);
    char lib_path[300];
    snprintf(lib_path, sizeof(lib_path), "%s%s", PKG_INSTALL_ROOT_DIR, COLORPRINT_LIB_PATH);

    lib_handler = dlopen(lib_path, RTLD_LAZY);
    if(!lib_handler) printf("Open %s failed!\n", lib_path);

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

