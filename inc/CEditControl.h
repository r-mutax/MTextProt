#include "CEditEngine.h"

enum EDIT_TYPE{
    COMMAND_MODE = 0
    ,INSERT_MODE
    ,QUIT_OPERATION
};

class CEditControl {
    CEditEngine* m_cediteng;
    char* m_filename;
    EDIT_TYPE m_mode;
public:
    CEditControl();
    ~CEditControl(){};
    void run();
    void command_loop();
    void init();
    void command_convert();
    void keygen_command_mode(char key);
    void keygen_insert_mode(char key);
    void keygen_commonkey(char key);
    bool set_filename(char* filename);
};
