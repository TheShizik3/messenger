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

class ServerThread
{
    private:
        std::string name;
        boost::asio::io_service io_s;
        boost::asio::ip::tcp::socket sock;
        boost::asio::ip::tcp::acceptor acceptor_; 
        Database &db;
        int port;
        bool atWork_;
        std::string id;

        std::string read();
        void send(const std::string text);
        static int countCallback(void* data, int argc, char** argv, char** col_names);
        Message readMessage();
        void sendMessage(Message message);
        static int columnsReader(void* data, int argc, 
                                    char** argv, char** colNames);
        static int columnReader(void* data, int argc, 
                                char** argv, char** colNames);
        void addMessage(const Message message);
        Message getMessage();
        void regUser(const QString password, const QString name);
        void loginUser(const QString password, const QString name);
    public:
        ServerThread(const int port, Database &database); 
        bool socketFull();
        bool databaseFull();
        void listen();
        void handleMessage();
        void handleSendMessage();
        void checkConnection();
        static bool fileExists(const std::string &filename);
        static std::string readFile(const std::string &filename);
        static void writeFile(const std::string &filename, const std::string data);
        bool atWork();
};