#include "message.h"
#include "key.h"
#include "console.h"
#include <queue>
#include <thread>
#include <mutex>

std::mutex mtx;

class CMessageEngine{
private:
    std::thread m_thread_key_monitoring;
    std::thread m_thread_display_monitoring;

    std::queue<MESSAGE> m_msg_pool;

    bool m_bRun;
    void key_monitoring();
    void display_monitoring();
    
    std::mutex mtx;

public:
    CMessageEngine();
    ~CMessageEngine();
    MESSAGE_ID GetMessage(MESSAGE& msg);
    void SendMessage(MESSAGE& msg);
};

namespace message_nsp
{
    CMessageEngine* msg_engine;
} // namespace msg

MESSAGE_ID GetMessage(MESSAGE& msg){
    return message_nsp::msg_engine->GetMessage(msg);
}

void SendMessage(MESSAGE& msg){
    message_nsp::msg_engine->SendMessage(msg);
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
        char input_key;
        input_key = get_vkcode();

        if(input_key){
            MESSAGE msg;
            msg.id = MM_KEYPRESS;
            msg.info = input_key;
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

void CMessageEngine::SendMessage(MESSAGE& msg){
    std::lock_guard<std::mutex>    ul(mtx);
    m_msg_pool.push(msg);
}

MESSAGE_ID CMessageEngine::GetMessage(MESSAGE& msg){
    
    enum MESSAGE_ID ret;
    
    while(1){
        mtx.lock();
        if(m_msg_pool.size() != 0){
            msg = m_msg_pool.front();
            m_msg_pool.pop();
            ret = msg.id;
            mtx.unlock();
            break;
        } else {
            // 30ミリ秒sleepする
            usleep(30000);
        }

        mtx.unlock();
    }
    return ret;

}