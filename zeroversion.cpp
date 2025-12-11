#include <fstream>
#include <yaml-cpp/yaml.h>
#include "sha256.h"
#include "user.h"
#include "chacha20.h"
#include "rsa.h"

void User::makeChat(const std::string name) {
    Message message(this->name, "server", "system", "GET SYSTEM FILE", "", "keys/"+name+".rsa");
    sendMessage(message);
    handleMessage();
    auto [key, nonce] = chacha20::generateKeys(32, 12);
    chacha20::saveKeys(key, nonce, 32, 12, "client/keys/"+name+".chacha20");
    Message newKeyMsg(this->name, name, "chats", "MAKE CHAT", rsa::encrypt(rsa::loadPublicKey("client/keys/"+name+".rsa"), readFile("client/keys/"+name+".chacha20")), "");
    boost::system::error_code err;
    auto [serverKey, serverNonce] = chacha20::loadKeys(32, 12, "client/keys/server.chacha20");
    send(chacha20::encrypt(serverKey, serverNonce, newKeyMsg.toStr()));
}
Message User::readMessage() {
    const std::string jsonStr = read();
    auto [serverKey, serverNonce] = chacha20::loadKeys(32, 12, "client/keys/server.chacha20");
    Message message(chacha20::encrypt(serverKey, serverNonce, jsonStr));
    return message;
}
std::string User::read() {
    boost::asio::streambuf buf;
    boost::asio::read_until(*sock, buf, "\0");
    return boost::asio::buffer_cast<const char*>(buf.data());
}
void User::send(const std::string text) {
    boost::system::error_code err;
    boost::asio::write(*sock, boost::asio::buffer(text+"\0"), err);
}
void User::sendMessage(Message message) {
    if (!fileExists("client/keys/"+message.to+".chacha20")) { makeChat(message.to); }
    auto [key, nonce] = chacha20::loadKeys(32, 12, "client/keys/"+message.to+".chacha20");
    auto [serverKey, serverNonce] = chacha20::loadKeys(32, 12, "client/keys/server.chacha20");
    message.text = chacha20::encrypt(key, nonce, message.text);
    send(chacha20::encrypt(serverKey, serverNonce, message.toStr()));
}
void User::addMessage(const Message message) {
    if (message.to == name) {
        if (message.type == "FILE") { 
            db->exec("INSERT INTO \"chats:"+message.from+"\" (from_, text, isFile, filename) VALUES (\'"+message.from+"\', \'"+message.filename+"\', true, \'client/downloads/"+message.filename+"\')");
        }
        else if (message.type == "MESSAGE") {
            db->exec("INSERT INTO \"chats:"+message.from+"\" (from_, text, isFile, filename) VALUES (\'"+message.from+"\', \'"+message.text+"\', false, \'"+message.filename+"\')");
        }
    }
    else if (message.from == name) {
        if (message.type == "FILE") {
            db->exec("INSERT INTO \"chats:"+message.to+"\" (from_, text, isFile, filename) VALUES (\'"+message.to+"\', \'"+message.filename+"\', true, \'client/downloads/"+message.filename+"\')");
        }
        else if (message.type == "MESSAGE") {
            db->exec("INSERT INTO \"chats:"+message.to+"\" (from_, text, isFile, filename) VALUES (\'"+message.to+"\', \'"+message.text+"\', false, \'"+message.filename+"\')");
        }
    }
    if (message.type == "FILE") {
        writeFile("client/downloads/"+message.filename, message.text);
    }
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
User::User(const std::string name, const std::string ip, 
        const int startPort, const int endPort) {
    boost::asio::io_service io_s;
    boost::asio::ip::tcp::socket sock(io_s);
    int port = startPort;

    // while (true) {
    //     try {
    //         sock.connect(boost::asio::ip::tcp::endpoint(boost::asio::ip::address::from_string(ip.c_str()), port));
    //         break;
    //     }
    //     catch (boost::system::system_error err) {}
    //     if (port <= endPort) {
    //         port++;
    //     }
    //     else {
    //         port = startPort;
    //     }
    // }
    this->name = name;
    // this->io_s = &io_s;
    // this->sock = &sock;
    db = new Database("client/db.db");
    // if (!fileExists("client/keys/server.chacha20")) {
    //     rsa::generateKeys("client/privateKey.rsa", "client/publicKey.rsa", 2048);
    //     Message rsaKey(name, "server", "system", "RSA KEY", readFile("client/publicKey.rsa"), "");
    //     send(rsaKey.toStr());
    //     Message reply = readMessage();
    //     writeFile("client/keys/server.chacha20", rsa::decrypt(rsa::loadPrivateKey("client/privateKey.rsa"), reply.text));
    // }
    atWork_ = true;
}
Q_INVOKABLE void User::handleSendMessage(const QString group, const QString to, const QString text, const QString filename) {
    Message message;
    if (filename.toStdString() == "") {
        message = Message(name, to.toStdString(), group.toStdString(), "MESSAGE", text.toStdString(), "");
    }
    else {
        message = Message(name, to.toStdString(), group.toStdString(), "FILE", readFile(filename.toStdString()), filename.toStdString());
    }
    sendMessage(message);     
    addMessage(message);                         
}
void User::handleMessage() {
    Message message = readMessage();
    if (message.type == "MESSAGE" or message.type == "FILE") {
        auto [key, nonce] = chacha20::loadKeys(32, 12, "client/keys/"+message.from+".chacha20");
        message.text = chacha20::encrypt(key, nonce, message.text);
        addMessage(message);
    }
    else if (message.type == "DELETE MESSAGE") {
        auto [key, nonce] = chacha20::loadKeys(32, 12, "client/keys/"+message.from+".chacha20");
        message.text = chacha20::encrypt(key, nonce, message.text);
        db->exec("DELETE FROM "+message.from+" WHERE rowid = "+message.text);
    }
    else if (message.type == "SYSTEM FILE") {
        auto [key, nonce] = chacha20::loadKeys(32, 12, "client/keys/"+message.from+".chacha20");
        message.text = chacha20::encrypt(key, nonce, message.text);
        writeFile("client/"+message.filename, message.text);
    }
    else if (message.type == "MAKE CHAT") {
        addChat(QString::fromStdString("chats"), QString::fromStdString(message.from));
        writeFile("client/keys/"+message.from+".chacha20", rsa::decrypt(rsa::loadPrivateKey("client/privateKey.rsa"), message.text));
    }
}
Q_INVOKABLE void User::deleteMessage(const QString group, const QString chat, const QString id) {
    db->exec("DELETE FROM \""+group.toStdString()+":"+chat.toStdString()+"\" WHERE rowid = "+id.toStdString());
    Message delMsg(name, chat.toStdString(), group.toStdString(), "DELETE MESSAGE", id.toStdString(), "");
    sendMessage(delMsg);
}
Q_INVOKABLE void User::deleteChat(const QString group, const QString name) {
    db->exec("DROP TABLE \""+group.toStdString()+":"+name.toStdString()+"\"");
    db->exec("DELETE FROM "+group.toStdString()+" WHERE name = "+name.toStdString());
}
Q_INVOKABLE void User::addChat(const QString group, const QString name) {
    db->exec("INSERT INTO "+group.toStdString()+" (name) VALUES (\'"+name.toStdString()+"\')");
    db->exec("CREATE TABLE \""+group.toStdString()+":"+name.toStdString()+"\" (from_ TEXT, text TEXT, isFile BOOLEAN, filename TEXT)");
}
Q_INVOKABLE void User::addGroup(const QString name) {
    db->exec("CREATE TABLE "+name.toStdString()+" (name TEXT)");
    db->exec("INSERT INTO groups (name) VALUES (\'"+name.toStdString()+"\')");
}
Q_INVOKABLE void User::deleteGroup(const QString name) {
    db->exec("DROP TABLE "+name.toStdString());
    db->exec("DELETE FROM groups WHERE name = "+name.toStdString());
}
Q_INVOKABLE bool User::checkData() {
    Message message("", "server", "system", "LOGIN", readFile("client/config.yaml"), "");
    sendMessage(message);
    message = readMessage();
    if (message.text == "succeed" and message.type == "LOGIN") {
        return true;
    }
    else { return false; }
}
Q_INVOKABLE bool User::regUser(const QString password, const QString name) {
    std::string stdPassword;
    YAML::Node config;
    stdPassword = sha256::hash(password.toStdString());
    config["password"] = stdPassword;
    config["name"] = name.toStdString();
    std::ofstream output("client/config.yaml");
    output << config;
    output.close();
    Message message("", "server", "system", "REG", readFile("client/config.yaml"), "");
    sendMessage(message);
    message = readMessage();
    if (message.text == "succeed" and message.type == "LOGIN") {
        return true;
    }
    else { return false; }
}
Q_INVOKABLE bool User::loginUser(const QString password, const QString name) {
    std::string stdPassword;
    YAML::Node config;
    stdPassword = sha256::hash(password.toStdString());
    config["password"] = stdPassword;
    config["name"] = name.toStdString();
    std::ofstream output("client/config.yaml");
    output << config;
    output.close();
    Message message("", "server", "system", "LOGIN", readFile("client/config.yaml"), "");
    sendMessage(message);
    message = readMessage();
    if (message.text == "succeed" and message.type == "LOGIN") {
        return true;
    }
    else { return false; }
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
Q_INVOKABLE void User::closeWindow() { 
    atWork_ = false;
}
bool User::atWork() {
    return atWork_;
}
QList<QMap<QString, QVariant>> User::getMessages(const std::string group, const std::string chat) {
    QList<QMap<QString, QVariant>> messages;
    db->exec("SELECT * FROM \""+group+":"+chat+"\"", columnsReader, &messages);
    return messages;
}
QStringList User::getChats(const std::string group) {
    QStringList chats;
    db->exec("SELECT name FROM "+group, columnReader, &chats);
    return chats;
}
QStringList User::getGroups() {
    QStringList groups;
    db->exec("SELECT name FROM groups", columnReader, &groups);
    return groups;
}
QList<QMap<QString, QVariant>> User::getGroup(const std::string name) {
    QStringList chats = getChats(name);
    QList<QMap<QString, QVariant>> res;
    QMap<QString, QVariant> chat;
    for (int i = 0; i < chats.size(); i++) {
        chat["name"] = chats[i];
        chat["messages"] = QVariant::fromValue(getMessages(name, chats[i].toStdString()));
        res.append(chat);
    }
    return res;
}
bool User::socketFull() {
    return (sock->available() > 0);
}
YAML::Node config = YAML::LoadFile("config.yaml");
        
        std::string name = config["name"].as<std::string>();  
