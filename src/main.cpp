#include <stdlib.h>
#include <stdio.h>
#include <list>
#include <string>
#include <iostream>
#include "mtxtprot.h"

#define clr             "\e[2J"

CLineMgr* cmng;

static std::list<std::string> filemap;

int main(int argc, char *argv[]){

    if(argc != 2){
        printf("[error]illeagal args\n");
    }

    cmng = new CLineMgr();
    cmng->openfile(argv[1]);

    printf(clr);                // 画面クリア
    printf("\e[1;1H");          // カーソルを左上に

    do {
        std::cout << cmng->GetCurrentLine() << std::endl;
    } while(cmng->MoveToNextLine());

    return 0;
}
