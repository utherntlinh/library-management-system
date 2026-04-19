#pragma once
#include "Member.h"
#include <vector>
#include <string>
using namespace std;

/*
 *  File: MemberService.h
 *  Author: [Your Name] - [Student ID]
 *  Module: Member
 *  Description:
 *      This class manages library member functionalities:
 *         - Register new account
 *         - Log in / Log out
 *         - Update user's personal profile
 *
 *      Member data is temporarily stored in a vector<Member>.
 *      When integrated with the Database module, this data will
 *      be connected to the SQL Member table.
 */

class MemberService {
private:
    vector<Member> members;       // Temporary in-memory data (mock database)
    Member* currentMember = nullptr; // Pointer to currently logged-in member

public:
    MemberService() = default;

    // ======== Core Functionalities ========
    bool registerMember(const string& name, const string& email,
        const string& password, const string& phone,
        const string& expiryDate);

    Member* login(const string& email, const string& password);

    bool updateProfile(const string& newName, const string& newPhone);

    void logout();

    // ======== Utility Methods ========
    bool isLoggedIn() const { return currentMember != nullptr; }
    Member* findByEmail(const string& email);
    Member* getCurrentMember() { return currentMember; }
};
