#include <iostream>
#include <fstream>
#include <string>
#include <list>

// エディタ用バッファ管理クラス
class CLineMgr{
    std::list<std::string> m_editbuf;
    uint16_t m_LineCount;
    uint16_t m_CurrentLine;
    std::list<std::string>::iterator m_itrCurrentLine;
public:
    CLineMgr();
    ~CLineMgr();
    bool openfile(std::string filename);
    bool openfile(char* filename);
    std::string GetCurrentLine();
    bool MoveToNextLine();
    bool MoveToPreviousLine();
};