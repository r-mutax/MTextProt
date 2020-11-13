#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <sys/time.h>
#include <thread>
#include <future>

#include "util.h"

//-----------------------------------------------------------------
// タイムアウト付き文字入力
//-----------------------------------------------------------------
int getchar_w(int timeout, bool& endflg)
{
    char c = 0;
    struct sigaction sa;
    struct itimerval itimer;

    endflg = true;

    // シグナルハンドラの設定

    if(sigaction(SIGALRM, nullptr, nullptr) != 0){
        //perror("sigaction");
        return 0;
    }
    // タイマーの設定
    itimer.it_value.tv_sec  = itimer.it_interval.tv_sec  = 0; // sec
    itimer.it_value.tv_usec = itimer.it_interval.tv_usec = timeout; // micro sec
    if(setitimer(ITIMER_REAL, &itimer, NULL) < 0) {
        //perror("setitimer");
        return 0;
    }
    // １文字入力
    read(1,&c,1);

    endflg = false;
    
    return c;
}

void getchar_t(std::promise<char> p){
    char c = getchar();
    p.set_value(c);
}

int getkey(KEY& keycode){

    bool endflg = false;

    do{
        std::promise<char> p;
        std::future<char> f = p.get_future();

        std::thread t(getchar_t, std::move(p));

        namespace chrono = std::chrono;
        chrono::steady_clock::time_point tp = chrono::steady_clock::now();
        std::future_status result = f.wait_until(tp + chrono::milliseconds(10));

        if (result != std::future_status::timeout) {
            char key = f.get();
            keycode.keys[keycode.keynum] = key;
            keycode.keynum++;
            if(keycode.keynum > 9){
                // キーコードの数が10を超えたら、たまり過ぎなので買える。
                break;
            }            
        }  else {
            endflg = true;
        }

        t.join();
    } while(!endflg);

    return 1;
}