#define _CRT_SECURE_NO_WARNINGS
#include "AuthenticationManager.h"
#include <iostream>
#include <ctime>
#include <iomanip>
#include <sstream>

AuthenticationManager::AuthenticationManager(std::shared_ptr<DatabaseManager> db)
    : dbManager(db), isLibrarianLoggedIn(false), isMemberLoggedIn(false) {
}

AuthenticationManager::~AuthenticationManager() {}

bool AuthenticationManager::LoginAsLibrarian(const std::string& username, const std::string& password) {
    if (!dbManager->AuthenticateLibrarian(username, password, currentLibrarian)) {
        std::cout << "Invalid username or password!" << std::endl;
        return false;
    }

    isLibrarianLoggedIn = true;
    std::cout << "Login successful! Welcome, " << currentLibrarian.Name << std::endl;
    return true;
}

bool AuthenticationManager::LoginAsMember(const std::string& email, const std::string& password) {
    Member member;
    if (!dbManager->GetMemberByEmail(email, member)) {
        std::cout << "Member not found!" << std::endl;
        return false;
    }

    // Test password
    if (member.Password != password) {
        std::cout << "Invalid password!" << std::endl;
        return false;
    }

    if (member.Status != "Active") {
        std::cout << "Your account is " << member.Status << ". Please contact the librarian." << std::endl;
        return false;
    }

    currentMember = member;
    isMemberLoggedIn = true;
    std::cout << "Login successful! Welcome, " << currentMember.Name << std::endl;
    return true;
}

bool AuthenticationManager::RegisterNewMember(const std::string& name, const std::string& email,
    const std::string& phone, const std::string& password) {
    Member existingMember;
    if (dbManager->GetMemberByEmail(email, existingMember)) {
        std::cout << "Email already registered!" << std::endl;
        return false;
    }

    time_t now = time(nullptr);
    tm* timeinfo = localtime(&now);
    timeinfo->tm_year += 1;
    mktime(timeinfo);

    std::ostringstream dateStream;
    dateStream << std::put_time(timeinfo, "%Y-%m-%d");

    Member newMember;
    newMember.Name = name;
    newMember.Email = email;
    newMember.Phone = phone;
    newMember.Password = password;
    newMember.Status = "Active";
    newMember.ExpiryDate = dateStream.str();

    if (!dbManager->AddMember(newMember)) {
        std::cout << "Failed to register member!" << std::endl;
        return false;
    }

    std::cout << "Registration successful! Please login with your email." << std::endl;
    return true;
}

bool AuthenticationManager::LogoutLibrarian() {
    if (!isLibrarianLoggedIn) {
        std::cout << "No librarian logged in!" << std::endl;
        return false;
    }

    isLibrarianLoggedIn = false;
    std::cout << "Logout successful!" << std::endl;
    return true;
}

bool AuthenticationManager::LogoutMember() {
    if (!isMemberLoggedIn) {
        std::cout << "No member logged in!" << std::endl;
        return false;
    }

    isMemberLoggedIn = false;
    std::cout << "Logout successful!" << std::endl;
    return true;
}

bool AuthenticationManager::IsLibrarianLoggedIn() const {
    return isLibrarianLoggedIn;
}

bool AuthenticationManager::IsMemberLoggedIn() const {
    return isMemberLoggedIn;
}

Librarian AuthenticationManager::GetCurrentLibrarian() const {
    return currentLibrarian;
}

Member AuthenticationManager::GetCurrentMember() const {
    return currentMember;
}

void AuthenticationManager::SetCurrentMember(const Member& member) {
    currentMember = member;
}

bool AuthenticationManager::AddDefaultLibrarian() {
    Librarian defaultLibrarian;
    defaultLibrarian.Name = "Admin";
    defaultLibrarian.Username = "admin";
    defaultLibrarian.Password = "admin123";

    return dbManager->AddLibrarian(defaultLibrarian);
}