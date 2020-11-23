#include "Logger.h"
#include <string>
#include <sstream>
#include <chrono>

FILE* fp = nullptr;

void RegisterLog(const char* func, std::string str){

    if(fp == nullptr){
        fp = fopen("log.txt","a");
    }

    if(fp == nullptr){
        return;
    }

    fseek(fp, 0, SEEK_END);

    std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();

    auto duration = now.time_since_epoch();
    auto mills = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();

    std::stringstream ss;
    ss << mills;

    std::string buf = func;
    buf = ss.str() + std::string(" ") + buf + std::string(" ") + str;
    fprintf(fp,buf.c_str());
    fprintf(fp,"\r\n");
}

void EndLog(){
    if(fp != nullptr){
        fclose(fp);
    }
}
