#include "database.h"
#include <iostream>

Database::Database(const std::string filename) {
    sqlite3_open(filename.c_str(), &db);
}
Database::Database() {}
Database::~Database() {
    sqlite3_close(db);
}
void Database::exec(const std::string request) {
    int rc = sqlite3_exec(db, request.c_str(), nullptr, nullptr, nullptr);
    sqlite3_exec(db, "COMMIT", nullptr, nullptr, nullptr);
}
void Database::exec(const std::string request, int (*callback)(void*, int, char**, char**), void* data) {  
    sqlite3_exec(db, request.c_str(), callback, data, nullptr);
    sqlite3_exec(db, "COMMIT", nullptr, nullptr, nullptr);
}
