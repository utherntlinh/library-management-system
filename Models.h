#ifndef MODELS_H
#define MODELS_H

#include <string>

// Librarian Entity
class Librarian {
public:
    int LibrarianID;
    std::string Name;
    std::string Username;
    std::string Password;

    Librarian() : LibrarianID(0) {}
    Librarian(int id, const std::string& name, const std::string& username, const std::string& password)
        : LibrarianID(id), Name(name), Username(username), Password(password) {
    }
};

// Member Entity
class Member {
public:
    int MemberID;
    std::string Name;
    std::string Email;
    std::string Phone;
    std::string Password;   // Fix 2: thêm field Password
    std::string Status;
    std::string ExpiryDate;

    Member() : MemberID(0) {}
    Member(int id, const std::string& name, const std::string& email, const std::string& phone,
        const std::string& password, const std::string& status, const std::string& expiryDate)
        : MemberID(id), Name(name), Email(email), Phone(phone),
        Password(password), Status(status), ExpiryDate(expiryDate) {
    }
};

// Book Entity
class Book {
public:
    int BookID;
    std::string ISBN;
    std::string Title;
    std::string Author;
    std::string Category;
    std::string Publisher;
    int Year;
    int Quantity;
    int AvailableQuantity;

    Book() : BookID(0), Year(0), Quantity(0), AvailableQuantity(0) {}
    Book(int id, const std::string& isbn, const std::string& title, const std::string& author,
        const std::string& category, const std::string& publisher, int year, int qty, int availQty)
        : BookID(id), ISBN(isbn), Title(title), Author(author), Category(category),
        Publisher(publisher), Year(year), Quantity(qty), AvailableQuantity(availQty) {
    }
};

// Loan Entity
class Loan {
public:
    int LoanID;
    int MemberID;
    int BookID;
    std::string BorrowDate;
    std::string DueDate;
    std::string ReturnDate;
    std::string Status;

    Loan() : LoanID(0), MemberID(0), BookID(0) {}
    Loan(int id, int memberId, int bookId, const std::string& borrowDate,
        const std::string& dueDate, const std::string& returnDate, const std::string& status)
        : LoanID(id), MemberID(memberId), BookID(bookId), BorrowDate(borrowDate),
        DueDate(dueDate), ReturnDate(returnDate), Status(status) {
    }
};

// Fine Entity
class Fine {
public:
    int FineID;
    int LoanID;
    double Amount;
    std::string Status;

    Fine() : FineID(0), LoanID(0), Amount(0.0) {}
    Fine(int id, int loanId, double amount, const std::string& status)
        : FineID(id), LoanID(loanId), Amount(amount), Status(status) {
    }
};

// Notification Entity
class Notification {
public:
    int NotificationID;
    int MemberID;
    std::string Message;
    std::string Date;
    std::string Status;

    Notification() : NotificationID(0), MemberID(0) {}
    Notification(int id, int memberId, const std::string& message, const std::string& date, const std::string& status)
        : NotificationID(id), MemberID(memberId), Message(message), Date(date), Status(status) {
    }
};

#endif // MODELS_H