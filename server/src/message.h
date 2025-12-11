#pragma once
#include <string>

class Message 
{
    private:
        std::string encodeBase64(const std::string text);
        std::string decodeBase64(const std::string text);
    public:
        std::string from, to, type, text;
        Message();
        Message(const std::string jsonStr);
        Message(const std::string from, 
                const std::string to,  
                const std::string type, 
                const std::string text);
        std::string toStr();
};
