#include <fstream>
#include <yaml-cpp/yaml.h>
#include <set>
#include <iostream>
#include "serverThread.h"
#include "sha256.h"

std::string ServerThread::read() {
    boost::asio::streambuf buf;
    boost::system::error_code err;
    auto n = boost::asio::read_until(sock, buf, '\0', err);
    std::string res = boost::asio::buffer_cast<const char*>(buf.data());
    if (n > 0) {
        res.resize(n-1);
    }
    return res;
}
void ServerThread::send(const std::string text) {
    boost::system::error_code err;
    const std::string res = text+'\0';
    boost::asio::write(sock, boost::asio::buffer(res), err);
}
int ServerThread::countCallback(void* data, int argc, char** argv, char** col_names) {
    int* count = static_cast<int*>(data);
    if (argc > 0 && argv[0]) {
        *count = std::atoi(argv[0]);
    }
    return 0;
}
Message ServerThread::readMessage() {
    Message msg(read());
    std::cout << msg.text << "\n";
    return msg;
}
void ServerThread::sendMessage(Message message) {
    send(message.toStr());
}
int ServerThread::columnReader(void* data, int argc, 
                            char** argv, char** colNames) {
    QStringList* list = static_cast<QStringList*>(data);
    QString value = QString::fromUtf8(argv[0]);
    list->append(value);
    
    return 0;
}
int ServerThread::columnsReader(void* data, int argc, 
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
void ServerThread::addMessage(const Message message) {
    if ((message.type == "MESSAGE") or (message.type == "NEW CHAT")) {
        db.exec("INSERT INTO "+message.to+" (from_, type, text) VALUES (\'"+message.from+"\', \'"+message.type+"\', \'"+message.text+"\')");
    }
}
Message ServerThread::getMessage() {
    QList<QMap<QString, QVariant>> messages;
    db.exec("SELECT * FROM "+name+" WHERE rowid = (SELECT MIN(rowid) FROM "+name+")", columnsReader, &messages);
    db.exec("DELETE FROM "+name+" WHERE rowid = (SELECT MIN(rowid) FROM "+name+")", columnsReader, &messages);
    return Message(messages[0]["from_"].toString().toStdString(), name, messages[0]["type"].toString().toStdString(), messages[0]["text"].toString().toStdString());
}
void ServerThread::regUser(const QString password, const QString name) {
    int count = 0;
    db.exec("SELECT COUNT(*) FROM sqlite_master WHERE type='table' AND name='" + name.toStdString(), countCallback, &count);
    if ((count == 0) and (name.toStdString() != "") and (password.toStdString() != "")) {
        db.exec("CREATE TABLE "+name.toStdString()+" (from_ TEXT, type TEXT, text TEXT)");
        db.exec("INSERT INTO users (name, password) VALUES (\'"+name.toStdString()+"\', \'"+password.toStdString()+"\')");
        this->name = name.toStdString();
        Message message("server", this->name, "REG", "SUCCEED");
        sendMessage(message);
    }
    else {
        Message message("server", "", "REG", "UNSUCCEED");
        sendMessage(message);
    }
}
void ServerThread::loginUser(const QString password, const QString name) {
    int count = 0;
    db.exec("SELECT COUNT(*) FROM sqlite_master WHERE type=\'table\' AND name=\'" + name.toStdString()+"\'", countCallback, &count);
    if ((count != 0) and (name.toStdString() != "") and (password.toStdString() != "")) {
        QList<QMap<QString, QVariant>> users;
        db.exec("SELECT * FROM users", columnsReader, &users);
        for (int i=0; i < users.length(); i++) {
            //std::cout << users[i]["name"].toString().toStdString() << "\n" << name.toStdString() << "\n" << (users[i]["name"].toString() == name) << "\n";
            if (users[i]["name"].toString() == name) {
                if (users[i]["password"].toString() == password) {
                    this->name = name.toStdString();
                    Message message("server", this->name, "LOGIN", "SUCCEED");
                    sendMessage(message);
                } else {
                    Message message("server", "", "LOGIN", "UNSUCCEED");
                    sendMessage(message);
                }
                return;
            }
        }        
    }
    Message message("server", "", "LOGIN", "UNSUCCEED");
    sendMessage(message);
}
bool ServerThread::socketFull() {
    return (sock.available() > 0);
}
bool ServerThread::databaseFull() {
    int count = 0;
    
    db.exec("SELECT COUNT(*) FROM " + name, countCallback, &count);
    return (count != 0);
}
ServerThread::ServerThread(const int port, Database &db_) : db(db_), acceptor_(io_s, 
              boost::asio::ip::tcp::endpoint(boost::asio::ip::address_v4::any(), port)), sock(io_s) {
    acceptor_.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
    id = std::to_string(port);
    this->port = port;
}
void ServerThread::listen() {
    acceptor_.listen();
    acceptor_.accept(sock);
    atWork_ = true;
}
void ServerThread::handleMessage() {
    Message message = readMessage();
    if (message.type == "LOGIN" or message.type == "REG") {
        writeFile(id, message.text);
        YAML::Node config = YAML::LoadFile(id);
        config["name"].as<std::string>();
        if (message.type == "LOGIN") {
            loginUser(QString::fromStdString(config["password"].as<std::string>()), QString::fromStdString(config["name"].as<std::string>()));
        } 
        else {
            regUser(QString::fromStdString(config["password"].as<std::string>()), QString::fromStdString(config["name"].as<std::string>()));
        }
    } 
    else if (message.type == "CHECK USER") {
        int count = 0;
        db.exec("SELECT COUNT(*) FROM sqlite_master WHERE type=\'table\' AND name=\'"+message.text+"\'", countCallback, &count);
        if (count != 0) {
            message.to = message.from;
            message.from = "server";
            message.text = "EXISTS";
            message.type = "CHECK USER";
        }
        else {
            message.to = message.from;
            message.from = "server";
            message.text = "UNEXISTS";
            message.type = "CHECK USER";
        }
        sendMessage(message);
    }
    else {
        addMessage(message);
    }
}
void ServerThread::handleSendMessage() {
    sendMessage(getMessage());
}
void ServerThread::checkConnection() {
    try {
        Message message("server", name, "CHECK", "");
        boost::system::error_code err;
        boost::asio::write(sock, boost::asio::buffer(message.toStr()), err);
        if (!err) {atWork_ = true;}
        else {atWork_ = false;}
    } catch (const std::exception& e) {
        std::cout << "???";
        atWork_ = false;
    }
    // char buffer[1];
    // boost::system::error_code ec;
    // sock.read_some(boost::asio::buffer(buffer), ec);
    // if (ec) {
    //     atWork_ = false;
    // }
    // else {
    //     atWork_ = true;
    // }
}
bool ServerThread::fileExists(const std::string &filename) {
    std::ifstream file(filename);
    return file.good();
}
std::string ServerThread::readFile(const std::string &filename) {
    std::ifstream file(filename, std::ios::binary);
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}
void ServerThread::writeFile(const std::string &filename, const std::string data) {
    std::ofstream file(filename, std::ios::binary);
    file.write(data.c_str(), data.size());
    file.close();
}
bool ServerThread::atWork() {
    // boost::system::error_code err;
    // boost::asio::const_buffer empty_buffer = boost::asio::buffer("", 0);
    // sock.write_some(empty_buffer, err);
    // if (err) {
    //     return false;
    // }
    // return true;
    // boost::system::error_code err;
    // sock.available(err);
    // if (err) {
    //     return false;
    // }
    // if (sock.is_open()) {
    //     return true;
    // }
    // return false;
    // char dump;
    // size_t read = sock.read_some(boost::asio::buffer(&dump, 0), err);    
    // return !err;
    return atWork_;
}
