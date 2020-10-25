#include "CEditControl.h"
#include "console.h"

CEditControl::CEditControl() : m_mode(COMMAND_MODE){
    m_cediteng = nullptr;
    m_filename = nullptr;
}

bool CEditControl::set_filename(char* filename){
    if(m_filename != nullptr){
        return false;
    }
    m_filename = filename;
    return true;
}

void CEditControl::run(){
    if(m_cediteng == nullptr){
        if(m_filename != ""){
            m_cediteng = new CEditEngine(m_filename);
        }
    }
    
    StartConsoleOperation();

    command_loop();

    EndConsoleOperation();    
}


void CEditControl::command_loop(){
    char input_key;

    m_cediteng->disp(0,10);

    while(input_key = getchar()){

        if(m_mode == COMMAND_MODE){
            keygen_command_mode(input_key);
        } else if(m_mode == INSERT_MODE){
            keygen_insert_mode(input_key);
        } else {
            break;
        }
    }
}

// コマンドモードでのキー処理
void CEditControl::keygen_command_mode(char key){

    switch(key){
        case 0x35:  // PAGE_UP
            m_cediteng->PageUp();
            break;
        case 0x36:  // PAGE_DOWM
            m_cediteng->PageDown();
            break;
        default:
            keygen_commonkey(key);
            break;
    }
}

// 挿入モードでのキー処理
void CEditControl::keygen_insert_mode(char key){
}

// 共通のキー処理
void CEditControl::keygen_commonkey(char key){
    switch (key)
    {
    case QUIT_CHAR:
        m_mode = QUIT_OPERATION;
        break;
    default:
        break;
    }
}