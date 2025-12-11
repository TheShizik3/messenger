#include <nlohmann/json.hpp>
#include <QByteArray>
#include <QString>
#include "message.h"

std::string Message::encodeBase64(const std::string text) {
    QByteArray byteArray(text.c_str(), text.length());
    QByteArray base64 = byteArray.toBase64();
    return std::string(base64.constData(), base64.length());
}
std::string Message::decodeBase64(const std::string text) {
    QByteArray base64(text.c_str(), text.length());
    QByteArray byteArray = QByteArray::fromBase64(base64);
    return std::string(byteArray.constData(), byteArray.length());
}
Message::Message(const std::string jsonStr) {
    nlohmann::json j = nlohmann::json::parse(jsonStr);
    this->from = j["from"];
    this->to = j["to"];
    this->type = j["type"];
    this->text = decodeBase64(j["text"]);
}
Message::Message(const std::string from, 
        const std::string to,
        const std::string type, 
        const std::string text) {
    this->from = from;
    this->to = to;
    this->type = type;
    this->text = text;
}
Message::Message() {}
std::string Message::toStr() {
    nlohmann::json j;
    
    j["from"] = this->from;
    j["to"] = this->to;
    j["type"] = this->type;
    j["text"] = encodeBase64(this->text);
    return j.dump();
}