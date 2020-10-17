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
    return true;
}

bool CLineMgr::MoveToPreviousLine(){
    if(m_itrCurrentLine == m_editbuf.begin()){
        return false;
    }

    m_itrCurrentLine--;
    return true;
}