#include "CLineMgr.h"

CLineMgr::CLineMgr() : m_LineCount ( 0 ){
}

bool CLineMgr::openfile(std::string filename){

    std::ifstream ifs(filename.c_str());

    if(ifs.fail()){
        return false;
    }

    std::string linebuf;
    while(getline(ifs, linebuf)){
        m_editbuf.emplace_back(linebuf);
    }

    m_itrCurrentLine = m_editbuf.begin();
    m_CurrentLine = 0;

    return true;
}

bool CLineMgr::openfile(char* filename){

    std::ifstream ifs(filename);

    if(ifs.fail()){
        return false;
    }

    std::string linebuf;
    while(getline(ifs, linebuf)){
        m_editbuf.emplace_back(linebuf);
    }

    m_itrCurrentLine = m_editbuf.begin();
    m_CurrentLine = 0;
    
    return true;
}

std::string CLineMgr::GetCurrentLine(){
    return *m_itrCurrentLine;
}

bool CLineMgr::MoveToNextLine(){
    auto chk = m_itrCurrentLine;
    chk++;

    if(chk == m_editbuf.end()){
        return false;
    }
    
    m_itrCurrentLine++;
    ++m_CurrentLine;
    return true;
}

bool CLineMgr::MoveToPreviousLine(){
    if(m_itrCurrentLine == m_editbuf.begin()){
        return false;
    }

    m_itrCurrentLine--;
    --m_CurrentLine;
    return true;
}

bool CLineMgr::SetCurrentLine(int CurrentLineNo){

    // エラーチェック
    int maxline = (int)m_editbuf.size();
    if(maxline < CurrentLineNo) return false;
    if(CurrentLineNo < 0) return false;

    auto itr = m_editbuf.begin();
    int i = 0;
    for(;itr != m_editbuf.end(); ++itr){
        if(i == CurrentLineNo)
            break;
        ++i;
    }
    m_itrCurrentLine = itr;
    m_CurrentLine = CurrentLineNo;
    return true;
}

int CLineMgr::GetCurrentLineNo(){
    return m_CurrentLine;
}

int CLineMgr::GetLineNum(){
    return (int)m_editbuf.size();
}