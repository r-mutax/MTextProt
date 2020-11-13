#include "message.h"
#include "util.h"
#include "console.h"
#include <queue>
#include <thread>
#include <mutex>

std::mutex mtx;

class CMessageEngine{
private:
    std::thread m_thread_key_monitoring;
    std::thread m_thread_display_monitoring;

    bool m_bRun;
    void key_monitoring();
    void display_monitoring();
    std::mutex mtx;

public:
    CMessageEngine();
    ~CMessageEngine();
};

namespace message_nsp
{
    std::queue<MESSAGE> msg_pool;
    CMessageEngine* msg_engine;
} // namespace msg

MESSAGE_ID GetMessage(MESSAGE& msg){

    enum MESSAGE_ID ret;
    while(1){
        mtx.lock();
        if(message_nsp::msg_pool.size() != 0){
            msg = message_nsp::msg_pool.front();
            message_nsp::msg_pool.pop();
            ret = msg.id;
            mtx.unlock();
            break;
        } else {
            // 30ミリ秒sleepする
            //nanosleep(30000);
        }

        mtx.unlock();
    }
    return ret;
}

void SendMessage(MESSAGE& msg){
    std::lock_guard<std::mutex>    ul(mtx);
    message_nsp::msg_pool.push(msg);
#define DEBUG
#ifdef DEBUG
    FILE* fp;
    fp = fopen("log.text","a");
    if(fp){
        fseek(fp, 0, SEEK_SET);
        fprintf(fp, "%d\r\n", msg.id);
        fclose(fp);
    }
#endif
}

void PrepareMessageLoop(){
    message_nsp::msg_engine = new CMessageEngine();
}


CMessageEngine::CMessageEngine()
     : m_thread_key_monitoring(&CMessageEngine::key_monitoring, this)
     //, m_thread_display_monitoring(&CMessageEngine::display_monitoring, this)
     , m_bRun(true)
{
}

CMessageEngine::~CMessageEngine(){
    m_bRun = false;
    m_thread_key_monitoring.join();
    m_thread_display_monitoring.join();
}

void CMessageEngine::key_monitoring(){

    while(1){
        KEY c;
        getkey(c);
        if(c.keynum > 0){
            MESSAGE msg;
            msg.id = MM_KEYPRESS;
            msg.info = c;
            SendMessage(msg);
        }
    }
}

void CMessageEngine::display_monitoring(){
    static MSize ms,bef;
    while(1){
        GetConsoleSize(&ms);

        if(ChkConsoleSizeChange(bef, ms)){
            ms = bef;
            MESSAGE msg;
            msg.id = MM_CHANGE_WINSIZE;
            msg.info = ms;
            SendMessage(msg);
        }
    }
}