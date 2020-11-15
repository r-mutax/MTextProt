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