#ifndef DATABASE_H
#define DATABASE_H

#include <string>
#include <vector>
#include <iostream>

class Database {
private:
    bool isConnected;

public:
    Database(const std::string& path = "LibraryManagement.db");
    ~Database();

    bool Connect();
    bool Disconnect();
    bool IsConnected() const;
    bool CreateSchema();
};

#endif // DATABASE_H