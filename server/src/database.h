#pragma once
#include <string>
#include <functional> 
#include <sqlite3.h>

class Database
{
    private:
        sqlite3* db;        
    public:
        Database(const std::string filename);
        Database();
        ~Database();
        void exec(const std::string request);
        void exec(const std::string request, int (*callback)(void*, int, char**, char**), void* data);
};
