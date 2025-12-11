#pragma once
#include <string>
#include <QObject>
#include <boost/asio.hpp>
#include <boost/system.hpp>
#include <functional>
#include <QStringList>
#include <QMap>
#include <QString>
#include <QList>
#include <QVariant>
#include "database.h"
#include "message.h"

class User : public QObject
{
    Q_OBJECT
    private:
        std::string name;
        boost::asio::io_service io_s;
        boost::asio::ip::tcp::socket sock;
        Database db;
        bool atWork_;

        std::string read();
        void send(const std::string text);
        Message readMessage();
        void makeChat(const std::string name);
        void sendMessage(Message message);
        static int columnsReader(void* data, int argc, 
                                    char** argv, char** colNames);
        static int columnReader(void* data, int argc, 
                                char** argv, char** colNames);
        void addMessage(const Message message);
    public:
        User(const std::string ip, const int startPort, const int endPort);
        QList<QMap<QString, QVariant>> getMessages(const std::string chat);
        QStringList getChats();
        bool socketFull();
        void handleMessage();
        Q_INVOKABLE void handleSendMessage(const QString to, const QString text);
        Q_INVOKABLE void addChat(const QString name);
        Q_INVOKABLE bool checkData();
        Q_INVOKABLE bool regUser(const QString password, const QString name);
        Q_INVOKABLE bool loginUser(const QString password, const QString name);
        Q_INVOKABLE void closeWindow(); 
        static bool fileExists(const std::string &filename);
        static std::string readFile(const std::string &filename);
        static void writeFile(const std::string &filename, const std::string data);
        bool atWork();
};