<<<<<<< HEAD
#ifndef AUTHENTICATION_MANAGER_H
#define AUTHENTICATION_MANAGER_H

#include "DatabaseManager.h"
#include "Models.h"
#include <memory>
#include <string>

=======
#pragma once
#include "DatabaseManager.h"
#include <string>
#include <memory>
 
>>>>>>> a4259e55f67550b64b47b127def27d1fb90a4043
class AuthenticationManager {
private:
    std::shared_ptr<DatabaseManager> dbManager;
    bool isLibrarianLoggedIn;
    bool isMemberLoggedIn;
    Librarian currentLibrarian;
    Member currentMember;
<<<<<<< HEAD

public:
    AuthenticationManager(std::shared_ptr<DatabaseManager> db);
    ~AuthenticationManager();

    bool LoginAsLibrarian(const std::string& username, const std::string& password);
    bool LoginAsMember(const std::string& email, const std::string& password);
    bool RegisterNewMember(const std::string& name, const std::string& email,
        const std::string& phone, const std::string& password);

    bool LogoutLibrarian();
    bool LogoutMember();

    bool IsLibrarianLoggedIn() const;
    bool IsMemberLoggedIn() const;

    Librarian GetCurrentLibrarian() const;
    Member GetCurrentMember() const;
    void SetCurrentMember(const Member& member);

    bool AddDefaultLibrarian();
};

#endif // AUTHENTICATION_MANAGER_H
=======
 
public:
    AuthenticationManager(std::shared_ptr<DatabaseManager> db);
    ~AuthenticationManager();
 
    bool LoginAsLibrarian(const std::string& username, const std::string& password);
    bool LoginAsMember(const std::string& email, const std::string& password);
    bool RegisterNewMember(const std::string& name, const std::string& email,
                           const std::string& phone, const std::string& password);
    bool LogoutLibrarian();
    bool LogoutMember();
 
    bool IsLibrarianLoggedIn() const;
    bool IsMemberLoggedIn() const;
 
    Librarian GetCurrentLibrarian() const;
    Member GetCurrentMember() const;
 
    bool AddDefaultLibrarian();
};
 
>>>>>>> a4259e55f67550b64b47b127def27d1fb90a4043
