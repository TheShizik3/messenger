#include <fstream>
#include <yaml-cpp/yaml.h>
#include "sha256.h"
#include "user.h"
#include <iostream>

std::string User::read() {
    boost::asio::streambuf buf;
    boost::system::error_code err;
    auto n = boost::asio::read_until(sock, buf, '\0', err);
    std::string res = boost::asio::buffer_cast<const char*>(buf.data());
    if (n > 0) {
        res.resize(n-1);
    }
    return res;
}
void User::send(const std::string text) {
    boost::system::error_code err;
    const std::string res = text+'\0';
    boost::asio::write(sock, boost::asio::buffer(res), err);
}
Message User::readMessage() {
    Message msg(read());
    std::cout << msg.text << "\n";
    return msg;
}
void User::sendMessage(Message message) {
    send(message.toStr());
}
int User::columnReader(void* data, int argc, 
                            char** argv, char** colNames) {
    QStringList* list = static_cast<QStringList*>(data);
    QString value = QString::fromUtf8(argv[0]);
    list->append(value);
    
    return 0;
}
int User::columnsReader(void* data, int argc, 
                            char** argv, char** colNames) {
    QList<QMap<QString, QVariant>>* list = static_cast<QList<QMap<QString, QVariant>>*>(data);
    QMap<QString, QVariant> row;
    QString column; 
    QVariant value;

    for (int i = 0; i < argc; i++) {
        column = QString::fromUtf8(colNames[i]);
        value = QString::fromUtf8(argv[i]);
        row[column] = value;
    }
    
    list->append(row);
    
    return 0;
}
void User::addMessage(const Message message) {
    if (message.to == name) {
        db.exec("INSERT INTO "+message.from+" (from_, text) VALUES (\'"+message.from+"\', \'"+message.text+"\')");
    }
    else if (message.from == name) {
        db.exec("INSERT INTO "+message.to+" (from_, text) VALUES (\'"+message.to+"\', \'"+message.text+"\')");
    }
}
void User::makeChat(const std::string name) {
    Message message(this->name, name, "NEW CHAT", "");
    send(message.toStr());
}
User::User(const std::string ip, const int startPort, const int endPort) : db("client/db.db"), sock(io_s) {
    boost::asio::io_service io_s;
    int port = startPort;

    while (true) {
            sock.connect(boost::asio::ip::tcp::endpoint(boost::asio::ip::address::from_string(ip.c_str()), port));
            break;
        if (port <= endPort) {
            port++;
        }
        else {
            port = startPort;
        }
    }
    atWork_ = true;
} 
QList<QMap<QString, QVariant>> User::getMessages(const std::string chat) {
    QList<QMap<QString, QVariant>> messages;
    db.exec("SELECT * FROM "+chat, columnsReader, &messages);
    return messages;
}
QStringList User::getChats() {
    QStringList chats; 
    db.exec("SELECT name FROM chats", columnReader, &chats);
    return chats;
}
bool User::socketFull() {
    return (sock.available() > 0);
}
void User::handleMessage() {
    Message message = readMessage();
    if (message.type == "NEW CHAT") {
        db.exec("CREATE TABLE "+message.from+" (from_ TEXT, text TEXT)");
        db.exec("INSERT INTO chats (name) VALUES (\'"+message.from+"\')");
    }
    else if (message.type == "MESSAGE") {
        addMessage(message);
    }
}
Q_INVOKABLE void User::handleSendMessage(const QString to, const QString text) {
    Message message(name, to.toStdString(), "MESSAGE", text.toStdString());
}
Q_INVOKABLE void User::addChat(const QString name) {
    Message message(this->name, "server", "CHECK USER", name.toStdString());
    sendMessage(message);
    message = readMessage();
    if ((message.type == "CHECK USER") and (message.text == "EXISTS")) {
        makeChat(name.toStdString());
        db.exec("CREATE TABLE "+name.toStdString()+" (from_ TEXT, text TEXT)");
        db.exec("INSERT INTO chats (name) VALUES (\'"+name.toStdString()+"\')");
    }
}
Q_INVOKABLE bool User::checkData() {
    Message message("", "server", "LOGIN", readFile("client/config.yaml"));
    sendMessage(message);
    message = readMessage();
    if (message.type == "LOGIN" and message.text == "SUCCEED") { 
        YAML::Node config = YAML::LoadFile("client/config.yaml");
        this->name = config["name"].as<std::string>();
        return 0; 
    }
    else { return 1; }
}
Q_INVOKABLE bool User::regUser(const QString password, const QString name) {
    YAML::Node config;
    config["password"] = sha256::hash(password.toStdString());
    config["name"] = name.toStdString();
    std::ofstream output("client/config.yaml");
    output << config;
    output.close();
    Message message("", "server", "REG", readFile("client/config.yaml"));
    sendMessage(message);
    message = readMessage();
    if (message.type == "REG" and message.text == "SUCCEED") { 
        this->name = name.toStdString();
        return 0; 
    }
    else { return 1; }
} 
Q_INVOKABLE bool User::loginUser(const QString password, const QString name) {
    YAML::Node config;
    config["password"] = sha256::hash(password.toStdString());
    config["name"] = name.toStdString();
    std::ofstream output("client/config.yaml");
    output << config;
    output.close();
    Message message("", "server", "LOGIN", readFile("client/config.yaml"));
    sendMessage(message);
    message = readMessage();
    if (message.type == "LOGIN" and message.text == "SUCCEED") { 
        this->name = name.toStdString();
        return 0; 
    }
    else { return 1; }
} 
Q_INVOKABLE void User::closeWindow() { 
    atWork_ = false;
}
bool User::fileExists(const std::string &filename) {
    std::ifstream file(filename);
    return file.good();
}
std::string User::readFile(const std::string &filename) {
    std::ifstream file(filename, std::ios::binary);
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}
void User::writeFile(const std::string &filename, const std::string data) {
    std::ofstream file(filename, std::ios::binary);
    file.write(data.c_str(), data.size());
    file.close();
}
bool User::atWork() {
    return atWork_;
}
