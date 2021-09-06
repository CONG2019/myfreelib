#include <stdio.h>
#include <stdlib.h>
#include "colorprint.h"
#define __USE_GNU
#include <dlfcn.h>  //need to add -ldl when compile

#define COLORPRINT_LIB_PATH "/usr/lib/libclprint.so"

static void atexit_a(void){
    printf("%s\n", __func__);
}

static void atexit_b(void){
    printf("%s\n", __func__);
}

// add -Wl,--export-dynamic at building
void y(void){
    printf("Symbol replacement. I am libdemo's y().\n");
}

void dldemo(void){
    void *lib_handler;
    void (*clprogress_print)(const char *, ...);
    char lib_path[300];
    Dl_info info;

    snprintf(lib_path, sizeof(lib_path), "%s%s", PKG_INSTALL_ROOT_DIR, COLORPRINT_LIB_PATH);

    atexit(atexit_a);   //called in the reverse order of their registration
    atexit(atexit_b);

    lib_handler = dlopen(lib_path, RTLD_LAZY);
    if(!lib_handler) printf("Open %s failed!\n", lib_path);

    clprogress_print = dlsym(lib_handler, "clprogress_print");
    if(!clprogress_print) printf("Can not find symbol clprogress_print.\n" );

    dladdr(clprogress_print, &info);
    printf("dli_fname=%s, dli_fbase=%p, dli_sname=%s, dli_saddr=%p\n", 
    info.dli_fname, info.dli_fbase, info.dli_sname, info.dli_saddr);

    clprogress_print("Success.\n");

    dlclose(lib_handler);
}

void __attribute__((constructor)) some_name_load(void)
{
	printf("%s\n", __func__);
}

void __attribute__((destructor)) some_name_unload(void)
{
	printf("%s\n", __func__);
}

int main(int argc, char const *argv[])
{
    dldemo();
#ifdef LIB_HIDDEN_TEST
    extern void lprint(void);
    //lprint();
#endif
    extern void x(void);
    x();

    extern void xyz(void);
    xyz();
    return 0;
}

