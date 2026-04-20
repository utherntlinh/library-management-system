#include "Database.h"

Database::Database(const std::string& path)
    : isConnected(false) {
}

Database::~Database() {
    isConnected = false;
}

bool Database::Connect() {
    isConnected = true;
    return true;
}

bool Database::Disconnect() {
    isConnected = false;
    return true;
}

bool Database::IsConnected() const {
    return isConnected;
}

bool Database::CreateSchema() {
    return true;
}