#include <thread>
#include <future>
#include <vector>

#include "key.h"

void getchar_t(std::promise<char> p){
    char c = getchar();
    p.set_value(c);
}

char vkcode_conv(std::vector<char> &keys){
    char vk_code = 0x00;
    
    switch((int)keys.size()){
        case 1:
            vk_code = keys[0];
            break;

        case 2:
            // none
            break;
        
        case 3:
            // 方向キーチェック
            if(0x1b == keys[0] && 0x5b == keys[1]){
                switch(keys[2]){
                    case 65:
                        vk_code = VK_UP;
                        break;
                    case 66:
                        vk_code = VK_DOWN;
                        break;
                    case 67:
                        vk_code = VK_RIGHT;
                        break;
                    case 68:
                        vk_code = VK_LEFT;
                        break;
                    default:
                        break;
                }
            }

            break;
    }

    return vk_code;
}

char get_vkcode(){

    bool endflg = false;

    std::vector<char>   keys;

    do{
        std::promise<char> p;
        std::future<char> f = p.get_future();

        std::thread t(getchar_t, std::move(p));

        namespace chrono = std::chrono;
        chrono::steady_clock::time_point tp = chrono::steady_clock::now();
        std::future_status result = f.wait_until(tp + chrono::milliseconds(10));

        if (result != std::future_status::timeout) {
            char key = f.get();

            if(key){
                keys.emplace_back(key);
                if((int) keys.size() > 9){
                    break;
                }
            }
        }  else {
            endflg = true;
        }
        t.join();
    } while(!endflg);

    return vkcode_conv(keys);
}