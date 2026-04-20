#ifndef MENU_SYSTEM_H
#define MENU_SYSTEM_H

#include "AuthenticationManager.h"
#include "CirculationManager.h"
#include "DatabaseManager.h"
#include <memory>

class MenuSystem {
private:
    std::shared_ptr<DatabaseManager> dbManager;
    std::shared_ptr<AuthenticationManager> authManager;
    std::shared_ptr<CirculationManager> circulationManager;

public:
    MenuSystem(std::shared_ptr<DatabaseManager> db);
    ~MenuSystem();

    void Start();

private:
    void DisplayMainMenu();
    void HandleMainMenuChoice(int choice);

    void LibrarianMenu();
    void HandleLibrarianMenuChoice(int choice);

    void MemberMenu();
    void HandleMemberMenuChoice(int choice);

    // Librarian functions
    void ManageBooks();
    void ManageMembers();
    void IssueReturn();
    void FinesAndNotifications();

    // Member functions
    void SearchCatalog();
    void ViewMyLoans();
    void ViewProfile();
    void UpdateProfile();

    // Utility functions
    void DisplayCatalog(const std::vector<Book>& books);
    int GetInput(const std::string& prompt);
    std::string GetStringInput(const std::string& prompt);
    double GetDoubleInput(const std::string& prompt);
    void PauseScreen();
};

#endif // MENU_SYSTEM_H