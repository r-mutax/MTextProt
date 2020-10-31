#include "CEditControl.h"
#include "console.h"

#define clr             "\e[2J"

CEditControl::CEditControl() : m_mode(COMMAND_MODE){
    m_cediteng = nullptr;
    m_filename = nullptr;
    m_iStartLine = 0;
    update_winsize();
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

void CEditControl::update_winsize(){

    MSize ms;
    if(GetConsoleSize(&ms)){
        m_iPageLine = ms.iHeight - 2;
        m_iConsoleRows = ms.iHeight;
        m_iConsoleColumns = ms.iWidth;
    } else {
        // 取得に失敗した場合は10固定にしとく。
        m_iPageLine = 10;
    }
}

void CEditControl::command_loop(){
    char input_key;

    disp();

    while(input_key = getchar()){

        update_winsize();

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
        case 'i':
            m_mode = INSERT_MODE;
            disp();
            break;
        default:
            keygen_commonkey(key);
            break;
    }
}

// 挿入モードでのキー処理
void CEditControl::keygen_insert_mode(char key){

    switch(key){
        default:
            keygen_commonkey(key);
            break;
    }
}

// 共通のキー処理
void CEditControl::keygen_commonkey(char key){
    switch (key)
    {
        case 0x1b:
            m_mode = COMMAND_MODE;
            disp();
            break;
        case QUIT_CHAR:
            m_mode = QUIT_OPERATION;
            break;
        case 0x35:  // PAGE_UP
            PageUp();
            break;
        case 0x36:  // PAGE_DOWM
            PageDown();
            break;
        default:
            break;
    }
}

void CEditControl::PageDown(){

    int nextStartLine = m_iStartLine + m_iPageLine;
    int maxline = m_cediteng->GetLineNum();
    if( maxline < nextStartLine) return;
    if(nextStartLine == m_iStartLine) return;

    disp(nextStartLine, m_iPageLine);

}
void CEditControl::PageUp(){

    int nextStartLine = m_iStartLine - m_iPageLine;
    if(nextStartLine < 0) nextStartLine = 0;
    if(nextStartLine == m_iStartLine) return;

    disp(nextStartLine, m_iPageLine);
}

void CEditControl::disp(int StartLine, int dispLineNum){
    disp_sub(StartLine, dispLineNum);
}

void CEditControl::disp(){
    disp_sub(m_iStartLine, m_iPageLine);
}

void CEditControl::disp_sub(int StartLine, int dispLineNum){

    // 現在の開始行の更新
    m_iStartLine = StartLine;
    LINES buf;
    m_cediteng->GetLines(buf, StartLine, dispLineNum);

    printf(clr);

    // step1 編集中のファイル名の表示
    disp_filename();

    printf("\e[2;1H");
    for(auto str : buf){
        std::cout << str << "\r\n";
    }

    disp_mode();

}

std::string CEditControl::GetFileDispName(){
    std::string DispFileName;
    std::stringstream ss;
    if(m_filename == ""){
        DispFileName = "New File.";
    } else {
        ss << std::left << std::setw(m_iConsoleColumns) << std::setfill(' ') << m_filename;
        DispFileName = ss.str();
    }
    return DispFileName;
}

void CEditControl::disp_filename(){
    std::string dispName = GetFileDispName();
    printf("\e[1;1H");
    printf("\033[47m");
    printf("\033[34m");
    std::cout << dispName << "\r\n";
    printf("\033[39m");
    printf("\033[49m");
}

void CEditControl::disp_mode(){
    std::string mode;
    if(m_mode == COMMAND_MODE){
        mode = "-- command --";
    } else {
        mode = "-- insert --";
    }

    int iCursorPos = m_iConsoleColumns - (int)mode.size();
    printf("\e[%ld;%dH", m_iConsoleRows, iCursorPos);
    std::cout << mode;
    printf("\e[%ld;1H", m_iConsoleRows);
}