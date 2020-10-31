#include "console.h"

// 端末設定保存用大域変数
struct termios CookedTermIos; // cooked モード用
struct termios RawTermIos;    // raw モード用

bool m_bInitStatus = false;

void InitConsoleOperation(){

    // 初期化済なら即リターン
    if(m_bInitStatus) return;

    // Cookedモードを取得
    tcgetattr(STDIN_FILENO, &CookedTermIos);

    // CookedモードをもとにRawモードの設定を作る
    RawTermIos = CookedTermIos;
    cfmakeraw(&RawTermIos);
    m_bInitStatus = true;
}

void StartConsoleOperation(){
    InitConsoleOperation();
    tcsetattr(STDIN_FILENO, 0, &RawTermIos);
}

void EndConsoleOperation(){
    InitConsoleOperation();
    tcsetattr(STDIN_FILENO, 0, &CookedTermIos);
}

bool GetConsoleSize(MSize* ms){
    struct winsize ws;
    bool ret = false;

     if( ioctl( STDOUT_FILENO, TIOCGWINSZ, &ws ) != -1 ) {
        ms->iHeight = ws.ws_row;
        ms->iWidth = ws.ws_col;
        ret = true;
    }
    return ret;
}