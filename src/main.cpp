#include <stdlib.h>
#include <stdio.h>

#define clr             "\e[2J"

int main(int argc, char *argv[]){

    printf(clr);                // 画面クリア
    printf("\e[1;1H");          // カーソルを左上に
    printf("\e[31m\e[47m");     // color　文字を赤、波形を白
    printf("hello,world!\n");   
    printf("\e[0m\e[0m");

    for(int i=0; i<argc; i++) {
        printf("receive !! > %s \n",argv[i]);
    }
    return 0;
}