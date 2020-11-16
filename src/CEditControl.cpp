#include "CEditControl.h"
#include "console.h"
#include "message.h"

#define clr             "\e[2J"

// ---------------------------
// コンストラクタ
// ---------------------------
CEditControl::CEditControl() : m_mode(COMMAND_MODE){
    m_cediteng = nullptr;
    m_filename = nullptr;
    m_iStartLine = 0;
    m_cursor.iRow = 2;
    m_cursor.iColumn = 1;
    update_winsize();
}

// ---------------------------
// ファイル名を設定する
// ---------------------------
bool CEditControl::set_filename(char* filename){
    if(m_filename != nullptr){
        return false;
    }
    m_filename = filename;
    return true;
}

// ---------------------------
// 実行
// ---------------------------
void CEditControl::run(){

    if(m_cediteng == nullptr){
        if(m_filename != ""){
            m_cediteng = new CEditEngine(m_filename);
        }
    }

    StartConsoleOperation();

    //message_loop();
    command_loop();

    EndConsoleOperation();
    disp_clear();
    m_cursor.iRow = 1;
    m_cursor.iRow = 1;
    disp_cursor();
}

void CEditControl::message_loop(){

    MESSAGE msg;
    // msg_id が 0(= MM_QUIT)になったらループを抜けて終了する
    while(int msg_id = MMGetMessage(msg) != MM_QUIT){
        if(msg_id == MM_ERROR){
            // エラー処理を入れる
        }
        message_procedure(msg);
    }
}

void CEditControl::message_procedure(MESSAGE& msg){
    
    switch(msg.id){
        case MM_KEYPRESS:
        {
            //char keydev = std::any_cast<char>(msg.info);
            //OnKeyPress(keydev);
            break;
        }
        case MM_CHANGE_WINSIZE:
        {
            //MSize ms = std::any_cast<MSize>(msg.info);
            //OnChangeWindowSize(ms);
           break;
        }
        default:
            break;
    }
}

void CEditControl::OnChangeWindowSize(MSize& ms){
    m_iPageLine = ms.iHeight - 2;
    m_iConsoleRows = ms.iHeight;
    m_iConsoleColumns = ms.iWidth;

    disp();
}

void CEditControl::OnKeyPress(KEY& key){

}

// ---------------------------
// ウィンドウのサイズを更新する
// ---------------------------
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

// // -----------------------------
// // キー入力を受け付けてループする
// // -----------------------------
void CEditControl::command_loop(){
    char input_key;

    disp();

    while(input_key = getchar()){

        update_winsize();
        if(!keygen(input_key))
            break;
    }
}

// ---------------------------
// キー処理
// ---------------------------
bool CEditControl::keygen(char key){
    if(m_mode == COMMAND_MODE){
        keygen_command_mode(key);
    } else if(m_mode == INSERT_MODE){
        keygen_insert_mode(key);
    } else if(m_mode == DEBUG_MODE){
        if(key == 0x1b){
            m_mode = COMMAND_MODE;
            disp();
        }
        return false;
    } else {
        return false;
    }

    return true;
}

// ---------------------------
// コマンドモード中のキー処理
// ---------------------------
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

// ---------------------------
// 挿入モードでのキー処理
// ---------------------------
void CEditControl::keygen_insert_mode(char key){

    switch(key){
        default:
            keygen_commonkey(key);
            break;
    }
}

// ----------------------------
// 共通のキー処理
// ----------------------------
void CEditControl::keygen_commonkey(char key){
    switch (key)
    {
        case 'd':
            m_mode = DEBUG_MODE;
            break;
        case 0x1b:  // ESC
            {
                // ここでタイマ割り込みをしないといけないのでは？
                char next_key = getchar();
                char next_next_key = getchar();
                if(next_key == 0x5b && isArrowKey(next_next_key)){
                    move_cursor(next_next_key);
                } else {
                    // カーソルキーじゃなかったら、エスケープの処理をして
                    // 先読みしたキーふたつを流す。
                    m_mode = COMMAND_MODE;
                    disp();
                    keygen(next_key);
                    keygen(next_next_key);
                }
            }
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

// -----------------------------
// ページダウン
// -----------------------------
void CEditControl::PageDown(){

    int nextStartLine = m_iStartLine + m_iPageLine;
    int maxline = m_cediteng->GetLineNum();
    if( maxline < nextStartLine) return;
    if(nextStartLine == m_iStartLine) return;

    disp(nextStartLine, m_iPageLine);

}

// ---------------------------
// ページアップ
// ---------------------------
void CEditControl::PageUp(){

    int nextStartLine = m_iStartLine - m_iPageLine;
    if(nextStartLine < 0) nextStartLine = 0;
    if(nextStartLine == m_iStartLine) return;

    disp(nextStartLine, m_iPageLine);
}

// ---------------------------
// 画面描画（範囲指定なし）
// ---------------------------
void CEditControl::disp(int StartLine, int dispLineNum){
    disp_sub(StartLine, dispLineNum);
}

// ---------------------------
// 画面描画（範囲指定あり）
// ---------------------------
void CEditControl::disp(){
    disp_sub(m_iStartLine, m_iPageLine);
}

// ---------------------------
// 画面描画サブ
// ---------------------------
void CEditControl::disp_sub(int StartLine, int dispLineNum){

    // 現在の開始行の更新
    m_iStartLine = StartLine;
    LINES buf;
    m_cediteng->GetLines(buf, StartLine, dispLineNum);

    disp_clear();

    // step1 編集中のファイル名の表示
    disp_filename();

    printf("\e[2;1H");
    for(auto str : buf){
        std::cout << str << "\r\n";
    }

    disp_mode();

    disp_cursor();
}

// ---------------------------
// ファイル名表示用文字列の作成
// ---------------------------
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

// ---------------------------
// ファイル名の表示を行う
// ---------------------------
void CEditControl::disp_filename(){
    std::string dispName = GetFileDispName();
    printf("\e[1;1H");
    printf("\033[47m");
    printf("\033[34m");
    std::cout << dispName << "\r\n";
    printf("\033[39m");
    printf("\033[49m");
}

// ---------------------------
// 編集モードの表示を行う
// ---------------------------
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

// ---------------------------
// カーソルを現在位置に表示する
// ---------------------------
void CEditControl::disp_cursor(){
    printf("\e[%d;%dH", m_cursor.iRow, m_cursor.iColumn);   
}

// ---------------------------
// 表示をクリアする
// ---------------------------
void CEditControl::disp_clear(){
    printf(clr);
}

// ---------------------------
// カーソルを動かす
// ---------------------------
void CEditControl::move_cursor(char dir){
    switch(dir){
        case UP: // up
            m_cursor.iRow -= 1;
            disp_cursor();
            break;
        case DOWN: // down
            m_cursor.iRow++;
            disp_cursor();
            break;
        case RIGHT: // right
            m_cursor.iColumn++;
            disp_cursor();
            break;
        case LEFT: // left
            m_cursor.iColumn = -1;
            disp_cursor();
            break;
        default:
            break;
    }
}

// ---------------------------
// 方向キー？
// ---------------------------
bool CEditControl::isArrowKey(char dir){
    
    bool ret = false;

    switch(dir){
        case UP:
        case DOWN:
        case RIGHT:
        case LEFT:
            ret = true;
            break;
        default:
            break;
    }

    return ret;

}
