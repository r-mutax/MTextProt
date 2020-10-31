#include "CEditEngine.h"
#include <sstream>
#include <iomanip>

enum EDIT_TYPE{
    COMMAND_MODE = 0
    ,INSERT_MODE
    ,QUIT_OPERATION
};

class CEditControl {
    CEditEngine* m_cediteng;
    char* m_filename;
    EDIT_TYPE m_mode;

    long m_iStartLine;
    long m_iPageLine;
    long m_iConsoleRows;
    long m_iConsoleColumns;
    void disp_sub(int StartLine, int dispLineNum);
    std::string GetFileDispName();
    void disp_filename();
    void disp_mode();

public:
    CEditControl();
    ~CEditControl(){};
    void run();
    void disp();
    void disp(int StartLine, int dispLineNum);

    void command_loop();
    void keygen_command_mode(char key);
    void keygen_insert_mode(char key);
    void keygen_commonkey(char key);
    bool set_filename(char* filename);
    void update_winsize();
    void PageUp();
    void PageDown();
};
