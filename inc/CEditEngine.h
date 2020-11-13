#include "CLineMgr.h"
#include "mtxtprot.h"

#define QUIT_CHAR 0x04 /* CTRL-D を押すと終了する．*/

typedef std::list<std::string> LINES;

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
    int m_CurrentLine;  // 表示中の1行目

public:
    CEditEngine(char* filename);
    ~CEditEngine(){};
    void GetLines(LINES &buf, int iStartLine, int iDispLineNum);
    long GetLineNum();
};
