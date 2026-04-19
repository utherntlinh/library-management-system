#pragma once
#include <iostream>
#include <string>
using namespace std;

/*
 *  File: Member.h
 *  Author: [Your Full Name] - [Student ID]
 *  Project: Library Management System
 *  Module: Member
 *  Description:
 *      This class represents a library member (user/reader)
 *      with fields described in the project's class diagram.
 *      A Member can register, log in, and update their profile.
 */

class Member {
private:
    int memberID;           // Unique ID for each member
    string name;            // Full name
    string email;           // Email address (unique identifier)
    string password;        // Login password
    string phone;           // Contact number
    string status;          // Active / Blocked
    string expiryDate;      // Library card expiry date (yyyy-mm-dd)

public:
    // ===== Constructors =====
    Member() = default;
    Member(int id, const string& n, const string& e, const string& pw,
        const string& ph, const string& s, const string& exp)
        : memberID(id), name(n), email(e), password(pw),
        phone(ph), status(s), expiryDate(exp) {
    }

    // ===== Getters =====
    int getID() const { return memberID; }
    string getName() const { return name; }
    string getEmail() const { return email; }
    string getPassword() const { return password; }
    string getPhone() const { return phone; }
    string getStatus() const { return status; }
    string getExpiryDate() const { return expiryDate; }

    // ===== Setters =====
    void setName(const string& n) { name = n; }
    void setPassword(const string& pw) { password = pw; }
    void setPhone(const string& ph) { phone = ph; }
    void setStatus(const string& s) { status = s; }
    void setExpiryDate(const string& d) { expiryDate = d; }

    // ===== Display member information =====
    void displayInfo() const {
        cout << "\n===== MEMBER INFORMATION =====\n";
        cout << "ID          : " << memberID << endl;
        cout << "Name        : " << name << endl;
        cout << "Email       : " << email << endl;
        cout << "Phone       : " << phone << endl;
        cout << "Status      : " << status << endl;
        cout << "Expiry Date : " << expiryDate << endl;
    }
};
