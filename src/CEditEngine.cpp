#include "CEditEngine.h"
#include "console.h"

#define clr             "\e[2J"

CEditEngine::CEditEngine(char* filename)
{
    m_cmng = new CLineMgr;
    m_cmng->openfile(filename);
    SetPageHeight(10);
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

    if(nextStartLine == m_cmng->GetCurrentLineNo())
    {
        printf("%d",m_cmng->GetCurrentLineNo());
        return;
    }
    
    disp(nextStartLine, m_pageline);
}

void CEditEngine::SetPageHeight(int height){
    m_pageline = height;
}

void CEditEngine::disp(){
    disp_sub(m_cmng->GetCurrentLineNo(), m_pageline);
}

void CEditEngine::disp(int StartLine, int dispLineNum){
    disp_sub(StartLine, dispLineNum);
}

void CEditEngine::disp_sub(int StartLine, int dispLineNum){
    
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
