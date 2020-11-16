#ifndef MTXTPROT
#define MTXTPROT

#include <string>
#include <iostream>
#include <stdio.h>
#include <any>

#include "vkeycode.h"

// 位置系定義
typedef struct _MPos {
    int iRow;
    int iColumn;
} MPos; // 座標値

typedef struct _MSize{
    int iWidth;
    int iHeight;
} MSize; // サイズ

// メッセージ系
enum MESSAGE_ID{
    MM_ERROR = -1
    , MM_QUIT = 0
    , MM_KEYPRESS
    , MM_CHANGE_WINSIZE
};

typedef struct MESSAGE{
    MESSAGE_ID  id;
    long        lParam;
    long        wParam;
} MESSAGE;

typedef struct _KEY{
    char keys[10];
    
    int keynum;
} KEY;






#endif