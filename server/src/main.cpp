#include "main.h"
// #include <array>
// #include <thread>
#include <functional>
#include <iostream>

void handleServerThread(const int port, Database &db) {
    ServerThread serverThread(port, db);
    serverThread.listen();
    // auto last_time = std::chrono::steady_clock::now();
    // auto now = std::chrono::steady_clock::now();
    // auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - last_time).count();
    while (serverThread.atWork()) {
        // now = std::chrono::steady_clock::now();
        // elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - last_time).count();
        // std::cout << elapsed << "\n";
        // if (elapsed > 5) {
        //     serverThread.checkConnection();
        //     last_time = std::chrono::steady_clock::now();
        // }
        if (serverThread.socketFull()) {
            serverThread.handleMessage();
        }
        if (serverThread.databaseFull()) {
            serverThread.handleSendMessage();
        }
    }
    std::cout <<"kakogo huya" << "\n";
}

int main() {
    Database db("db.db");
    // std::array<std::thread, 20> threads;
    
    // for(int i = 1234; i < 1254; i++) {
    //     threads[i] = std::thread(handleServerThread, i, std::ref(db));
    // }
    // for (auto& thread : threads) {
    //     thread.join();
    // }
    while (true) {
        handleServerThread(1234, std::ref(db));
    }
    return 0;
}