#include <iostream>
#include "DatabaseManager.h"
#include "MenuSystem.h"

using namespace std;

// Khai báo hàm thêm dữ liệu test
void InsertTestData(std::shared_ptr<DatabaseManager> dbManager);

int main() {
    auto dbManager = make_shared<DatabaseManager>("LibraryManagement.db");
    
    if (!dbManager->Initialize()) {
        cerr << "Failed to initialize database!" << endl;
        return 1;
    }
    
    // Add default librarian
    Librarian admin;
    admin.LibrarianID = 1;
    admin.Name = "Admin";
    admin.Username = "admin";
    admin.Password = "admin123";
    dbManager->AddLibrarian(admin);
    
    cout << "\n===================================" << endl;
    cout << "  LIBRARY MANAGEMENT SYSTEM" << endl;
    cout << "===================================" << endl;
    
    // Tự động thêm dữ liệu test mỗi lần chạy
    cout << "\n[*] Loading test data..." << endl;
    InsertTestData(dbManager);
    
    // Create menu system
    MenuSystem menu(dbManager);
    menu.Start();
    
    return 0;
}