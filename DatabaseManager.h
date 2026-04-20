#ifndef DATABASE_MANAGER_H
#define DATABASE_MANAGER_H

#include "Database.h"
#include "Models.h"
#include <vector>
#include <memory>

class DatabaseManager {
private:
    std::shared_ptr<Database> db;

    // In-Memory Storage
    std::vector<Librarian> librarians;
    std::vector<Member> members;
    std::vector<Book> books;
    std::vector<Loan> loans;
    std::vector<Fine> fines;
    std::vector<Notification> notifications;

public:
    DatabaseManager(const std::string& dbPath = "LibraryManagement.db");
    ~DatabaseManager();

    bool Initialize();

    // Member operations
    bool AddMember(const Member& member);
    bool GetMemberByID(int id, Member& member);
    bool GetMemberByEmail(const std::string& email, Member& member);
    bool GetAllMembers(std::vector<Member>& members);
    bool UpdateMember(const Member& member);
    bool DeleteMember(int id);

    // Book operations
    bool AddBook(const Book& book);
    bool GetBookByID(int id, Book& book);
    bool GetAllBooks(std::vector<Book>& books);
    bool SearchBooks(const std::string& searchTerm, std::vector<Book>& books);
    bool UpdateBook(const Book& book);
    bool DeleteBook(int id);

    // Loan operations
    bool AddLoan(const Loan& loan);
    bool GetLoanByID(int id, Loan& loan);
    bool GetLoansByMemberID(int memberId, std::vector<Loan>& loans);
    bool UpdateLoan(const Loan& loan);
    bool GetActiveLoansByMemberID(int memberId, std::vector<Loan>& loans);

    // Fine operations
    bool AddFine(const Fine& fine);
    bool GetFinesByLoanID(int loanId, std::vector<Fine>& fines);
    bool UpdateFineStatus(int fineId, const std::string& status);
    bool GetUnpaidFinesByMemberID(int memberId, std::vector<Fine>& fines);
    bool GetAllFinesByMemberID(int memberId, std::vector<Fine>& fines);

    // Notification operations
    bool AddNotification(const Notification& notification);
    bool GetNotificationsByMemberID(int memberId, std::vector<Notification>& notifications);
    bool UpdateNotificationStatus(int notificationId, const std::string& status);

    // Authentication
    bool AuthenticateLibrarian(const std::string& username, const std::string& password, Librarian& librarian);
    bool AddLibrarian(const Librarian& librarian);
};

#endif // DATABASE_MANAGER_H