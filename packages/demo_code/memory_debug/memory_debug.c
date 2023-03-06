#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

void usage(int argc, char *argv[]){
    printf("%s options\n"
"leaks            memery leaks\n"
"overflow         heap/stack buffer overflow\n"
"use_after_free   heap use after free\n"
"double_free      double free", argv[0]);
}

void leaks(){
    char *ptr = malloc(10);
    if(ptr){
        printf("malloc(10)\n");
    }
}

char heap_tmp[10];
void overflow(){
#if 0
    char tmp[10]={0,1,2,3,4,5,6,7,8,9};
#endif

#if 1
    char *tmp = malloc(10);
#endif

    printf("%c\n", tmp[10]);
    //printf("%c\n", heap_tmp[10000]);
}

void use_after_free(){
    char *tmp = malloc(10);

    free(tmp);

    tmp[0] = 1;
}

void double_free(){
    char *tmp = malloc(10);

    free(tmp);
    free(tmp);
}

int main(int argc, char *argv[]){
    if(argc != 2){
        usage(argc, argv);
        return 0;
    }

    if(!strcmp(argv[1], "leaks")){
        leaks();
    }else if(!strcmp(argv[1], "overflow")){
        overflow();
    }else if(!strcmp(argv[1], "use_after_free")){
        use_after_free();
    }else if(!strcmp(argv[1], "double_free")){
        double_free();
    }else{
        usage(argc, argv);
    }

    return 0;
}
