#include "mtxtprot.h"
#include "CEditEngine.h"

int main(int argc, char *argv[]){

    // stage 0 エラーチェック
    if(argc != 2){
        printf("[error]illeagal args\n");
        return -1;
    }

    CEditEngine cedit(argv[1]);
    cedit.run();

    // char c;
    // while((c = getchar()) != QUIT_CHAR){
    //     if(isprint(c)){
    //         putchar(c);
    //     } else {
    //         printf("<%02x>",c);
    //         if(c == '\n') fputs("\r\n",stdout);
    //     }
    // }

    return 0;
}
