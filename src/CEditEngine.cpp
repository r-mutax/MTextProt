#include "CEditEngine.h"
#include "console.h"

#define clr             "\e[2J"

CEditEngine::CEditEngine(char* filename)
{
    m_cmng = new CLineMgr;
    m_cmng->openfile(filename);
    SetPageheight(10);
}

void CEditEngine::run(){
    
    
    StartConsoleOperation();

    command_loop();

    EndConsoleOperation();    
}

void CEditEngine::command_loop(){

    int dispStartRow = 0;

    disp(dispStartRow, m_pageline);
    char input_key;
    while(input_key = getchar()){
        if(input_key == QUIT_CHAR){
            break;
        } else if(input_key == 0x35){   // Page Up
            PageUp();
        } else if(input_key == 0x36){   // Page Down
            PageDown();
        }
    }
}

void CEditEngine::disp(int StartLine, int dispLineNum){
    
    m_cmng->SetCurrentLine(StartLine);

    printf(clr);
    printf("\e[1;1H");

    int RemainDispNum = dispLineNum;
    do{
        std::cout << m_cmng->GetCurrentLine() << "\r\n";
        --RemainDispNum;
        if(RemainDispNum == 0) break;
    } while(m_cmng->MoveToNextLine());
}

void CEditEngine::PageDown(){
    
    int nextStartLine = m_cmng->GetCurrentLineNo() + 1;
    int maxline = m_cmng->GetLineNum();
    if( maxline <= nextStartLine){
        nextStartLine = maxline - m_pageline + 1;
        if(nextStartLine < 0){
            nextStartLine = 2;
        }
    }

    if(nextStartLine == m_cmng->GetCurrentLineNo()) return;

    disp(nextStartLine, m_pageline);
}

void CEditEngine::PageUp(){
    
    int nextStartLine = m_cmng->GetCurrentLineNo() - m_pageline - m_pageline;
    if(nextStartLine < 0){
        nextStartLine = 0;
    }

    if(nextStartLine == m_cmng->GetCurrentLineNo()) return;
    
    disp(nextStartLine, m_pageline);
}

void CEditEngine::SetPageheight(int height){
    m_pageline = height;
}