#include "CEditEngine.h"
#include "console.h"


CEditEngine::CEditEngine(char* filename)
    : m_CurrentLine(0)
{
    m_cmng = new CLineMgr;
    m_cmng->openfile(filename);
}

void CEditEngine::GetLines(LINES &buf, int iStartLine, int iDispLineNum){
    
    buf.clear();
    m_cmng->SetCurrentLine(iStartLine);

    do{
        buf.emplace_back(m_cmng->GetCurrentLine());
        if(buf.size() == iDispLineNum) return;
    } while(m_cmng->MoveToNextLine());

}

long CEditEngine::GetLineNum(){
    return m_cmng->GetLineNum();
}
