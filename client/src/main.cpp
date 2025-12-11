#include <QGuiApplication> 
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QScreen>
#include <QThread>
#include <boost/asio.hpp>
#include <boost/system.hpp>
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <openssl/err.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/bn.h>
#include <iostream>
#include <fstream>
#include <exception>
#include "main.h"

int main(int argc, char *argv[]) {
    OpenSSL_add_all_algorithms();
    ERR_load_crypto_strings();
    
    User user("127.0.0.1", 1234, 1254);
    QGuiApplication app(argc, argv);
    QScreen *screen = app.primaryScreen();  
    QQmlApplicationEngine engine;

    int screenWidth = screen->geometry().width();
    int screenHeight = screen->geometry().height();
    QStringList chats = user.getChats();
    engine.rootContext()->setContextProperty("chatsNames", QVariant::fromValue(chats));
    QMap<QString, QList<QMap<QString, QVariant>>> messages; 
    for (int i = 0; i < chats.size(); i++) {
        messages[chats[i]] = user.getMessages(chats[i].toStdString());
    }
    engine.rootContext()->setContextProperty("messages", QVariant::fromValue(messages));
    engine.rootContext()->setContextProperty("screenWidth", screenWidth);
    engine.rootContext()->setContextProperty("screenHeight", screenHeight);
    engine.rootContext()->setContextProperty(QStringLiteral("user"), &user);

    const QUrl url(QStringLiteral("qrc:/client/qml/main.qml"));
    engine.load(url); 
    while (user.atWork()) {
        if (user.socketFull()) {
            user.handleMessage();
            chats = user.getChats();
            engine.rootContext()->setContextProperty("chatsNames", QVariant::fromValue(chats));
            for (int i = 0; i < chats.size(); i++) {
                messages[chats[i]] = user.getMessages(chats[i].toStdString());
            }
            engine.rootContext()->setContextProperty("messages", QVariant::fromValue(messages));
        }
        app.processEvents();
        QThread::msleep(10);
    }
    EVP_cleanup();
    ERR_free_strings();
    return 0;
}
