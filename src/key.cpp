#include <vector>
#include <sys/ioctl.h>
#include <unistd.h>


#include "key.h"
#include "mtxtprot.h"

bool kbhit(){
    int byteswaiting;
    ioctl(0, FIONREAD, &byteswaiting);
    return byteswaiting > 0;
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

    std::vector<char>   keybuf;
    bool inputend = false;

    while(1){
        if(kbhit()){
            char key[10] = {};
            read(0, key, 10);
            
            for(int i = 0; i < 10; i++){
                if(key[i] == 0) break;
                keybuf.emplace_back(key[i]);
            }
        } else {
            inputend = true;
        }

        if((inputend || (int)keybuf.size() > 9) && (int)keybuf.size() != 0){
            break;
        }

        usleep(30000);
    }

    return vkcode_conv(keybuf);
}

