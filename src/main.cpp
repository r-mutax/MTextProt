#include "mtxtprot.h"
#include "message.h"
#include "CEditControl.h"

int main(int argc, char *argv[]){

    // stage 0 エラーチェック
    if(argc < 2){
        printf("[error]illeagal args\n");
        return -1;
    }

    //PrepareMessageLoop();

    CEditControl cedit;
    if(argc > 1){
        // 第2引数 = ファイル名があるなら
        cedit.set_filename(argv[1]);
    }
    
    cedit.run();

    return 0;
}
