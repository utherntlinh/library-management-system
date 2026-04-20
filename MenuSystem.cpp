#define _CRT_SECURE_NO_WARNINGS
#include "MenuSystem.h"
#include <iostream>
#include <iomanip>
#include <limits>
#include <ctime>
#include <sstream>

using namespace std;

MenuSystem::MenuSystem(std::shared_ptr<DatabaseManager> db)
    : dbManager(db) {
    authManager = std::make_shared<AuthenticationManager>(db);
    circulationManager = std::make_shared<CirculationManager>(db);
}

MenuSystem::~MenuSystem() {}

void MenuSystem::Start() {
    DisplayMainMenu();
}

void MenuSystem::DisplayMainMenu() {
    int choice = 0;
    while (true) {
        cout << "\n========== LIBRARY MANAGEMENT SYSTEM ==========" << endl;
        cout << "1. Login as Librarian" << endl;
        cout << "2. Login as Member" << endl;
        cout << "3. Register as New Member" << endl;
        cout << "4. Exit" << endl;
        cout << "=============================================" << endl;
        choice = GetInput("Choose an option: ");
        if (choice == 4) {
            cout << "Thank you for using Library Management System. Goodbye!" << endl;
            break;
        }
        HandleMainMenuChoice(choice);
    }
}

void MenuSystem::HandleMainMenuChoice(int choice) {
    std::string username, password, email, name, phone;
    switch (choice) {
    case 1: {
        cout << "\n===== LIBRARIAN LOGIN =====" << endl;
        username = GetStringInput("Username: ");
        password = GetStringInput("Password: ");
        if (authManager->LoginAsLibrarian(username, password)) {
            LibrarianMenu();
            authManager->LogoutLibrarian();
        }
        break;
    }
    case 2: {
        cout << "\n===== MEMBER LOGIN =====" << endl;
        email = GetStringInput("Email: ");
        password = GetStringInput("Password: ");
        if (authManager->LoginAsMember(email, password)) {
            MemberMenu();
            authManager->LogoutMember();
        }
        break;
    }
    case 3: {
        cout << "\n===== NEW MEMBER REGISTRATION =====" << endl;
        name = GetStringInput("Full Name: ");
        email = GetStringInput("Email: ");
        phone = GetStringInput("Phone: ");
        password = GetStringInput("Password: ");
        authManager->RegisterNewMember(name, email, phone, password);
        PauseScreen();
        break;
    }
    default:
        cout << "Invalid choice! Please try again." << endl;
    }
}

// ===== LIBRARIAN MENU =====
void MenuSystem::LibrarianMenu() {
    int choice = 0;
    while (true) {
        std::vector<Book> sumBooks;
        dbManager->GetAllBooks(sumBooks);
        int totalBooks = (int)sumBooks.size();
        int totalAvail = 0;
        for (const auto& b : sumBooks) totalAvail += b.AvailableQuantity;

        std::vector<Member> sumMembers;
        dbManager->GetAllMembers(sumMembers);
        int totalMembers = (int)sumMembers.size();
        int activeMembers = 0;
        for (const auto& mb : sumMembers) if (mb.Status == "Active") activeMembers++;

        time_t nowT = time(nullptr);
        struct tm tmT = {};
        localtime_s(&tmT, &nowT);
        std::ostringstream ossT;
        ossT << std::put_time(&tmT, "%Y-%m-%d");
        std::string todayStr = ossT.str();

        int activeLoans = 0, overdueLoans = 0;
        for (const auto& mb : sumMembers) {
            std::vector<Loan> mLoans;
            dbManager->GetActiveLoansByMemberID(mb.MemberID, mLoans);
            for (const auto& ln : mLoans) {
                activeLoans++;
                if (ln.DueDate < todayStr) overdueLoans++;
            }
        }

        cout << "\n========== LIBRARIAN DASHBOARD ==========" << endl;
        cout << "Welcome, " << authManager->GetCurrentLibrarian().Name << endl;
        cout << "------------------------------------------" << endl;
        cout << "  SUMMARY" << endl;
        cout << "  - Total Books   : " << totalBooks << " titles (" << totalAvail << " available)" << endl;
        cout << "  - Total Members : " << totalMembers << " (" << activeMembers << " active)" << endl;
        cout << "  - Active Loans  : " << activeLoans << endl;
        cout << "  - Overdue Cases : " << overdueLoans << endl;
        cout << "------------------------------------------" << endl;
        cout << "1. Manage Books" << endl;
        cout << "2. Manage Members" << endl;
        cout << "3. Issue/Return" << endl;
        cout << "4. Fines & Notifications" << endl;
        cout << "5. Logout" << endl;
        cout << "========================================" << endl;

        choice = GetInput("Choose an option: ");
        HandleLibrarianMenuChoice(choice);
        if (choice == 5) break;
    }
}

void MenuSystem::HandleLibrarianMenuChoice(int choice) {
    switch (choice) {
    case 1: ManageBooks();           break;
    case 2: ManageMembers();         break;
    case 3: IssueReturn();           break;
    case 4: FinesAndNotifications(); break;
    case 5: cout << "Logging out..." << endl; break;
    default: cout << "Invalid choice! Please try again." << endl;
    }
}

// ===== MEMBER MENU =====
void MenuSystem::MemberMenu() {
    int choice = 0;
    while (true) {
        Member curMember = authManager->GetCurrentMember();
        std::vector<Loan> sumLoans;
        circulationManager->GetMemberLoans(curMember.MemberID, sumLoans);

        int borrowed = 0, overdue = 0;
        time_t nowT = time(nullptr);
        struct tm tmT = {};
        localtime_s(&tmT, &nowT);
        std::ostringstream ossT;
        ossT << std::put_time(&tmT, "%Y-%m-%d");
        std::string todayStr = ossT.str();

        for (const auto& ln : sumLoans) {
            if (ln.Status == "Borrowing") {
                borrowed++;
                if (ln.DueDate < todayStr) overdue++;
            }
        }

        double totalUnpaid = circulationManager->GetTotalUnpaidFines(curMember.MemberID);
        double totalPaid = circulationManager->GetTotalPaidFines(curMember.MemberID);

        cout << "\n========== MEMBER DASHBOARD ==========" << endl;
        cout << "Welcome, " << curMember.Name << endl;
        cout << "------------------------------------------" << endl;
        cout << "  SUMMARY" << endl;
        cout << "  - Borrowed Books : " << borrowed << endl;
        cout << "  - Overdue Books  : " << overdue << endl;
        if (totalUnpaid > 0)
            cout << "  - Unpaid Fines   : " << totalUnpaid << " VND  *** Please pay! ***" << endl;
        if (totalPaid > 0)
            cout << "  - Paid Fines     : " << totalPaid << " VND  (cleared)" << endl;
        cout << "  - Account Status : " << curMember.Status << endl;
        cout << "------------------------------------------" << endl;
        cout << "1. Search Catalog" << endl;
        cout << "2. View My Loans" << endl;
        cout << "3. View Profile" << endl;
        cout << "4. Update Profile" << endl;
        cout << "5. Logout" << endl;
        cout << "=====================================" << endl;

        choice = GetInput("Choose an option: ");
        HandleMemberMenuChoice(choice);
        if (choice == 5) break;
    }
}

void MenuSystem::HandleMemberMenuChoice(int choice) {
    switch (choice) {
    case 1: SearchCatalog(); break;
    case 2: ViewMyLoans();   break;
    case 3: ViewProfile();   break;
    case 4: UpdateProfile(); break;
    case 5: cout << "Logging out..." << endl; break;
    default: cout << "Invalid choice! Please try again." << endl;
    }
}

// ===== MANAGE BOOKS =====
void MenuSystem::ManageBooks() {
    // Hiện danh sách sách trước (Interface Design)
    std::vector<Book> allBooks;
    dbManager->GetAllBooks(allBooks);
    if (!allBooks.empty()) DisplayCatalog(allBooks);

    cout << "\n===== MANAGE BOOKS =====" << endl;
    cout << "1. Add New Book" << endl;
    cout << "2. Update Book" << endl;
    cout << "3. Delete Book" << endl;
    cout << "4. Back" << endl;

    int choice = GetInput("Choose an option: ");

    switch (choice) {
    case 1: {
        cout << "\n--- Add New Book ---" << endl;
        Book newBook;
        newBook.BookID = 0;
        newBook.ISBN = "";
        newBook.Title = GetStringInput("Title: ");
        newBook.Author = GetStringInput("Author: ");
        newBook.Category = GetStringInput("Category: ");
        newBook.Publisher = GetStringInput("Publisher: ");
        newBook.Year = GetInput("Year: ");
        newBook.Quantity = GetInput("Quantity: ");
        newBook.AvailableQuantity = newBook.Quantity;

        if (dbManager->AddBook(newBook))
            cout << "[OK] Book added successfully!" << endl;
        else
            cout << "[ERROR] Failed to add book!" << endl;
        PauseScreen();
        break;
    }
    case 2: {
        cout << "\n--- Update Book ---" << endl;
        int bookId = GetInput("Enter Book ID to update: ");
        Book book;
        if (dbManager->GetBookByID(bookId, book)) {
            string input;
            input = GetStringInput("Title (current: " + book.Title + ", Enter to keep): ");
            if (!input.empty()) book.Title = input;

            input = GetStringInput("Author (current: " + book.Author + ", Enter to keep): ");
            if (!input.empty()) book.Author = input;

            input = GetStringInput("Category (current: " + book.Category + ", Enter to keep): ");
            if (!input.empty()) book.Category = input;

            input = GetStringInput("Publisher (current: " + book.Publisher + ", Enter to keep): ");
            if (!input.empty()) book.Publisher = input;

            int newQty = GetInput("Quantity (current: " + to_string(book.Quantity) + ", 0 to keep): ");
            if (newQty > 0) book.Quantity = newQty;

            if (dbManager->UpdateBook(book))
                cout << "[OK] Book updated successfully!" << endl;
            else
                cout << "[ERROR] Failed to update book!" << endl;
        }
        else {
            cout << "[ERROR] Book not found!" << endl;
        }
        PauseScreen();
        break;
    }
    case 3: {
        cout << "\n--- Delete Book ---" << endl;
        int bookId = GetInput("Enter Book ID to delete: ");
        Book book;
        if (dbManager->GetBookByID(bookId, book)) {
            cout << "Book: " << book.Title << endl;
            cout << "Are you sure? (y/n): ";
            char confirm;
            cin >> confirm;
            cin.ignore();
            if (confirm == 'y' || confirm == 'Y') {
                if (dbManager->DeleteBook(bookId))
                    cout << "[OK] Book deleted successfully!" << endl;
                else
                    cout << "[ERROR] Failed to delete book!" << endl;
            }
            else {
                cout << "Deletion cancelled." << endl;
            }
        }
        else {
            cout << "[ERROR] Book not found!" << endl;
        }
        PauseScreen();
        break;
    }
    case 4:
        break;
    default:
        cout << "Invalid choice!" << endl;
    }
}

// ===== MANAGE MEMBERS =====
void MenuSystem::ManageMembers() {
    // Hiện danh sách member trước (Interface Design)
    std::vector<Member> allMembers;
    dbManager->GetAllMembers(allMembers);
    if (!allMembers.empty()) {
        cout << "\n--- Member List ---" << endl;
        cout << setw(5) << "ID"
            << setw(20) << "Name"
            << setw(25) << "Email"
            << setw(12) << "Phone"
            << setw(10) << "Status"
            << setw(12) << "Expiry" << endl;
        cout << string(84, '-') << endl;
        for (const auto& m : allMembers) {
            cout << setw(5) << m.MemberID
                << setw(20) << m.Name
                << setw(25) << m.Email
                << setw(12) << m.Phone
                << setw(10) << m.Status
                << setw(12) << m.ExpiryDate << endl;
        }
    }

    cout << "\n===== MANAGE MEMBERS =====" << endl;
    cout << "1. Add New Member" << endl;
    cout << "2. Update Member Status (Block/Active)" << endl;
    cout << "3. Renew Member Card" << endl;
    cout << "4. Back" << endl;

    int choice = GetInput("Choose an option: ");

    switch (choice) {
    case 1: {
        cout << "\n--- Add New Member ---" << endl;
        string name = GetStringInput("Full Name: ");
        string email = GetStringInput("Email: ");
        string phone = GetStringInput("Phone: ");
        string password = GetStringInput("Password: ");

        if (authManager->RegisterNewMember(name, email, phone, password))
            cout << "[OK] Member added successfully!" << endl;
        else
            cout << "[ERROR] Failed to add member!" << endl;
        PauseScreen();
        break;
    }
    case 2: {
        int memberId = GetInput("Enter Member ID: ");
        Member member;
        if (dbManager->GetMemberByID(memberId, member)) {
            cout << "Member: " << member.Name << " | Status: " << member.Status << endl;
            cout << "1. Active\n2. Blocked\n3. Expired" << endl;
            int s = GetInput("Choose: ");
            if (s == 1) member.Status = "Active";
            else if (s == 2) member.Status = "Blocked";
            else if (s == 3) member.Status = "Expired";
            else { cout << "Invalid choice!" << endl; return; }

            if (dbManager->UpdateMember(member))
                cout << "[OK] Status updated to: " << member.Status << endl;
            else
                cout << "[ERROR] Failed to update!" << endl;
        }
        else {
            cout << "[ERROR] Member not found!" << endl;
        }
        PauseScreen();
        break;
    }
    case 3: {
        cout << "\n--- Renew Member Card ---" << endl;
        int memberId = GetInput("Enter Member ID: ");
        Member member;
        if (dbManager->GetMemberByID(memberId, member)) {
            cout << "Member: " << member.Name << " | Current Expiry: " << member.ExpiryDate << endl;

            time_t now = time(nullptr);
            tm timeinfo = {};
            localtime_s(&timeinfo, &now);
            timeinfo.tm_year += 1;
            mktime(&timeinfo);
            std::ostringstream ds;
            ds << std::put_time(&timeinfo, "%Y-%m-%d");
            member.ExpiryDate = ds.str();
            member.Status = "Active";

            if (dbManager->UpdateMember(member))
                cout << "[OK] Card renewed! New Expiry: " << member.ExpiryDate << endl;
            else
                cout << "[ERROR] Failed to renew!" << endl;
        }
        else {
            cout << "[ERROR] Member not found!" << endl;
        }
        PauseScreen();
        break;
    }
    case 4:
        break;
    default:
        cout << "Invalid choice!" << endl;
    }
}

// ===== ISSUE/RETURN =====
void MenuSystem::IssueReturn() {
    // Hiện danh sách loan hiện tại (Interface Design)
    cout << "\n--- Current Loans ---" << endl;
    cout << setw(8) << "LoanID"
        << setw(8) << "BookID"
        << setw(10) << "MemberID"
        << setw(13) << "Borrow Date"
        << setw(13) << "Due Date"
        << setw(12) << "Status" << endl;
    cout << string(64, '-') << endl;

    std::vector<Member> allMembers;
    dbManager->GetAllMembers(allMembers);
    for (const auto& mb : allMembers) {
        std::vector<Loan> mLoans;
        dbManager->GetLoansByMemberID(mb.MemberID, mLoans);
        for (const auto& ln : mLoans) {
            cout << setw(8) << ln.LoanID
                << setw(8) << ln.BookID
                << setw(10) << ln.MemberID
                << setw(13) << ln.BorrowDate
                << setw(13) << ln.DueDate
                << setw(12) << ln.Status << endl;
        }
    }

    cout << "\n===== ISSUE/RETURN =====" << endl;
    cout << "1. Issue Book to Member" << endl;
    cout << "2. Return Book from Member" << endl;
    cout << "3. Back" << endl;

    int choice = GetInput("Choose an option: ");

    switch (choice) {
    case 1: {
        cout << "\n--- Issue Book ---" << endl;
        int memberId = GetInput("Enter Member ID: ");
        Member member;
        if (!dbManager->GetMemberByID(memberId, member)) {
            cout << "[ERROR] Member not found!" << endl;
            PauseScreen(); break;
        }
        cout << "Member: " << member.Name << " (Status: " << member.Status << ")" << endl;

        int bookId = GetInput("Enter Book ID: ");
        Book book;
        if (!dbManager->GetBookByID(bookId, book)) {
            cout << "[ERROR] Book not found!" << endl;
            PauseScreen(); break;
        }
        cout << "Book: " << book.Title << " (Available: " << book.AvailableQuantity << ")" << endl;

        string errorMsg;
        if (circulationManager->IssueBook(memberId, bookId, errorMsg))
            cout << "[OK] Book issued successfully!" << endl;
        else
            cout << "[ERROR] " << errorMsg << endl;
        PauseScreen();
        break;
    }
    case 2: {
        cout << "\n--- Return Book ---" << endl;
        int loanId = GetInput("Enter Loan ID: ");
        string errorMsg;
        if (circulationManager->ReturnBook(loanId, errorMsg))
            cout << "[OK] Book returned successfully!" << endl;
        else
            cout << "[ERROR] " << errorMsg << endl;
        PauseScreen();
        break;
    }
    case 3:
        break;
    default:
        cout << "Invalid choice!" << endl;
    }
}

// ===== FINES & NOTIFICATIONS =====
// Hiện cả 2 bảng cùng lúc (Interface Design)
void MenuSystem::FinesAndNotifications() {
    cout << "\n===== FINES & NOTIFICATIONS =====" << endl;
    int memberId = GetInput("Enter Member ID: ");

    // Bảng FINES
    std::vector<Fine> allFines;
    dbManager->GetAllFinesByMemberID(memberId, allFines);

    cout << "\n--- FINES ---" << endl;
    if (allFines.empty()) {
        cout << "  No fines for this member." << endl;
    }
    else {
        cout << setw(8) << "Fine ID"
            << setw(10) << "Loan ID"
            << setw(12) << "Amount"
            << setw(10) << "Status" << endl;
        cout << string(40, '-') << endl;
        double totalUnpaid = 0.0;
        for (const auto& f : allFines) {
            cout << setw(8) << f.FineID
                << setw(10) << f.LoanID
                << setw(12) << f.Amount
                << setw(10) << f.Status << endl;
            if (f.Status == "Unpaid") totalUnpaid += f.Amount;
        }
        if (totalUnpaid > 0)
            cout << "  Total Unpaid: " << totalUnpaid << " VND" << endl;
    }

    // Bảng NOTIFICATIONS
    std::vector<Notification> notifications;
    dbManager->GetNotificationsByMemberID(memberId, notifications);

    cout << "\n--- NOTIFICATIONS ---" << endl;
    if (notifications.empty()) {
        cout << "  No notifications for this member." << endl;
    }
    else {
        cout << setw(5) << "ID"
            << setw(13) << "Date"
            << setw(45) << "Message"
            << setw(10) << "Status" << endl;
        cout << string(73, '-') << endl;
        for (const auto& n : notifications) {
            cout << setw(5) << n.NotificationID
                << setw(13) << n.Date
                << setw(45) << n.Message
                << setw(10) << n.Status << endl;
        }
    }

    // Xử lý thanh toán fine
    if (!allFines.empty()) {
        int fineId = GetInput("\nEnter Fine ID to mark as Paid (0 to skip): ");
        if (fineId > 0) {
            if (dbManager->UpdateFineStatus(fineId, "Paid"))
                cout << "[OK] Fine marked as Paid!" << endl;
            else
                cout << "[ERROR] Failed to update fine!" << endl;
        }
    }

    PauseScreen();
}

// ===== SEARCH CATALOG =====
void MenuSystem::SearchCatalog() {
    cout << "\n===== SEARCH CATALOG =====" << endl;
    string searchTerm = GetStringInput("Enter search term (Title/Author/Category): ");

    std::vector<Book> books;
    if (dbManager->SearchBooks(searchTerm, books)) {
        if (books.empty())
            cout << "No books found!" << endl;
        else
            DisplayCatalog(books);
    }
    else {
        cout << "Search failed!" << endl;
    }
    PauseScreen();
}

// ===== VIEW MY LOANS =====
void MenuSystem::ViewMyLoans() {
    Member currentMember = authManager->GetCurrentMember();
    std::vector<Loan> loans;

    if (!circulationManager->GetMemberLoans(currentMember.MemberID, loans)) {
        cout << "Failed to retrieve loans!" << endl;
        PauseScreen();
        return;
    }

    if (loans.empty()) {
        cout << "\nYou have no loans!" << endl;
        PauseScreen();
        return;
    }

    time_t nowT = time(nullptr);
    struct tm tmT = {};
    localtime_s(&tmT, &nowT);
    std::ostringstream ossT;
    ossT << std::put_time(&tmT, "%Y-%m-%d");
    std::string todayStr = ossT.str();

    cout << "\n===== MY LOANS =====" << endl;
    cout << setw(8) << "Loan ID"
        << setw(25) << "Title"
        << setw(13) << "Borrow Date"
        << setw(13) << "Due Date"
        << setw(12) << "Status" << endl;
    cout << string(71, '-') << endl;

    for (const auto& loan : loans) {
        Book book;
        string title = "[Unknown]";
        if (dbManager->GetBookByID(loan.BookID, book)) title = book.Title;
        if (title.length() > 22) title = title.substr(0, 19) + "...";

        string statusDisplay = loan.Status;
        if (loan.Status == "Borrowing" && loan.DueDate < todayStr)
            statusDisplay = "Overdue";

        cout << setw(8) << loan.LoanID
            << setw(25) << title
            << setw(13) << loan.BorrowDate
            << setw(13) << loan.DueDate
            << setw(12) << statusDisplay << endl;
    }
    PauseScreen();
}

// ===== VIEW PROFILE =====
void MenuSystem::ViewProfile() {
    Member currentMember = authManager->GetCurrentMember();
    cout << "\n===== MY PROFILE =====" << endl;
    cout << "Member ID  : " << currentMember.MemberID << endl;
    cout << "Name       : " << currentMember.Name << endl;
    cout << "Email      : " << currentMember.Email << endl;
    cout << "Phone      : " << currentMember.Phone << endl;
    cout << "Status     : " << currentMember.Status << endl;
    cout << "Expiry Date: " << currentMember.ExpiryDate << endl;
    PauseScreen();
}

// ===== UPDATE PROFILE =====
void MenuSystem::UpdateProfile() {
    Member currentMember = authManager->GetCurrentMember();
    cout << "\n===== UPDATE PROFILE =====" << endl;
    cout << "(Press Enter to keep current value)" << endl;

    string newName = GetStringInput("Name (current: " + currentMember.Name + "): ");
    if (!newName.empty()) currentMember.Name = newName;

    string newPhone = GetStringInput("Phone (current: " + currentMember.Phone + "): ");
    if (!newPhone.empty()) currentMember.Phone = newPhone;

    if (dbManager->UpdateMember(currentMember)) {
        Member updated;
        dbManager->GetMemberByID(currentMember.MemberID, updated);
        authManager->SetCurrentMember(updated);
        cout << "[OK] Profile updated successfully!" << endl;
    }
    else {
        cout << "[ERROR] Failed to update profile!" << endl;
    }
    PauseScreen();
}

// ===== DISPLAY CATALOG =====
void MenuSystem::DisplayCatalog(const std::vector<Book>& books) {
    const int W_ID = 5;
    const int W_ISBN = 7;
    const int W_TITLE = 22;
    const int W_AUTH = 18;
    const int W_CAT = 12;
    const int W_PUB = 16;
    const int W_YEAR = 6;
    const int W_QTY = 5;
    const int W_AVAIL = 15;

    auto shorten = [](const std::string& s, size_t max) -> std::string {
        if (s.length() <= max) return s;
        if (max <= 3) return s.substr(0, max);
        return s.substr(0, max - 3) + "...";
        };

    cout << "\n--- Book Catalog ---" << endl;
    cout << left
        << setw(W_ID) << "ID"
        << setw(W_ISBN) << "ISBN"
        << setw(W_TITLE) << "Title"
        << setw(W_AUTH) << "Author"
        << setw(W_CAT) << "Category"
        << setw(W_PUB) << "Publisher"
        << right
        << setw(W_YEAR) << "Year"
        << setw(W_QTY) << "Qty"
        << setw(W_AVAIL) << "Availability"
        << endl;
    cout << string(W_ID + W_ISBN + W_TITLE + W_AUTH + W_CAT + W_PUB + W_YEAR + W_QTY + W_AVAIL, '-') << endl;

    for (const auto& book : books) {
        string avail = (book.AvailableQuantity > 0)
            ? "Available(" + to_string(book.AvailableQuantity) + ")"
            : "Out of stock";

        cout << left
            << setw(W_ID) << book.BookID
            << setw(W_ISBN) << shorten(book.ISBN, W_ISBN - 1)
            << setw(W_TITLE) << shorten(book.Title, W_TITLE - 1)
            << setw(W_AUTH) << shorten(book.Author, W_AUTH - 1)
            << setw(W_CAT) << shorten(book.Category, W_CAT - 1)
            << setw(W_PUB) << shorten(book.Publisher, W_PUB - 1)
            << right
            << setw(W_YEAR) << book.Year
            << setw(W_QTY) << book.Quantity
            << setw(W_AVAIL) << avail
            << endl;
    }
}

// ===== UTILITIES =====
int MenuSystem::GetInput(const std::string& prompt) {
    int input;
    while (true) {
        cout << prompt;
        if (cin >> input) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return input;
        }
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input. Please enter a number." << endl;
    }
}

void MenuSystem::PauseScreen() {
    cout << "\n[Press Enter to continue...]";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

std::string MenuSystem::GetStringInput(const std::string& prompt) {
    string input;
    cout << prompt;
    getline(cin, input);
    return input;
}

double MenuSystem::GetDoubleInput(const std::string& prompt) {
    double input;
    while (true) {
        cout << prompt;
        if (cin >> input) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return input;
        }
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input. Please enter a number." << endl;
    }
}