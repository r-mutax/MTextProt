#include "CLineMgr.h"

#define QUIT_CHAR 0x04 /* CTRL-D を押すと終了する．*/

enum ED_MESSAGE{
    FILE_OPEN = 0
    ,FILE_CLOSE
    ,PAGE_UP
    ,PAGE_DOWN
    ,QUIT
};

typedef struct _message{
    ED_MESSAGE mes;
}message;

class CEditEngine{
    CLineMgr* m_cmng;
    int m_pageline;

    void disp_sub(int StartLine, int dispLineNum);

public:
    CEditEngine(char* filename);
    ~CEditEngine(){};
    void disp(int StartLine, int dispLineNum);
    void disp();
    void SetPageHeight(int height);
    void PageUp();
    void PageDown();
};
