#include "CLineMgr.h"

#define QUIT_CHAR 0x04 /* CTRL-D を押すと終了する．*/

class CEditEngine{
    CLineMgr* m_cmng;
    int m_pageline;

public:
    CEditEngine(char* filename);
    ~CEditEngine(){};
    void command_loop();
    void run();
    void disp(int StartLine, int dispLineNum);
    int GetNextPageStartLine();
    void SetPageheight(int height);
    void PageUp();
    void PageDown();
};
