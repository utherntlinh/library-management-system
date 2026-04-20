#include "DatabaseManager.h"
#include <algorithm>

DatabaseManager::DatabaseManager(const std::string& dbPath) {
    db = std::make_shared<Database>(dbPath);
}

DatabaseManager::~DatabaseManager() {}

bool DatabaseManager::Initialize() {
    if (!db->Connect()) {
        return false;
    }

    if (!db->CreateSchema()) {
        return false;
    }

    return true;
}

// ===== MEMBER OPERATIONS =====
bool DatabaseManager::AddMember(const Member& member) {
    Member newMember = member;

    if (newMember.MemberID == 0) {
        newMember.MemberID = (int)members.size() + 1;
    }

    members.push_back(newMember);
    return true;
}

bool DatabaseManager::GetMemberByID(int id, Member& member) {
    for (const auto& m : members) {
        if (m.MemberID == id) {
            member = m;
            return true;
        }
    }
    return false;
}

bool DatabaseManager::GetMemberByEmail(const std::string& email, Member& member) {
    for (const auto& m : members) {
        if (m.Email == email) {
            member = m;
            return true;
        }
    }
    return false;
}

bool DatabaseManager::GetAllMembers(std::vector<Member>& memberList) {
    memberList = members;
    return true;
}

bool DatabaseManager::UpdateMember(const Member& member) {
    for (auto& m : members) {
        if (m.MemberID == member.MemberID) {
            m = member;
            return true;
        }
    }
    return false;
}

bool DatabaseManager::DeleteMember(int id) {
    for (auto it = members.begin(); it != members.end(); ++it) {
        if (it->MemberID == id) {
            members.erase(it);
            return true;
        }
    }
    return false;
}

// ===== BOOK OPERATIONS =====
bool DatabaseManager::AddBook(const Book& book) {
    Book newBook = book;

    // Auto-assign BookID if = 0
    if (newBook.BookID == 0) {
        newBook.BookID = (int)books.size() + 1;
    }

    // Auto-assign ISBN if empty (001, 002, 003, ...)
    if (newBook.ISBN.empty()) {
        int isbnNum = (int)books.size() + 1;
        char isbnBuffer[20];
        snprintf(isbnBuffer, sizeof(isbnBuffer), "%03d", isbnNum);
        newBook.ISBN = std::string(isbnBuffer);
    }

    books.push_back(newBook);
    return true;
}

bool DatabaseManager::GetBookByID(int id, Book& book) {
    for (const auto& b : books) {
        if (b.BookID == id) {
            book = b;
            return true;
        }
    }
    return false;
}

bool DatabaseManager::GetAllBooks(std::vector<Book>& bookList) {
    bookList = books;
    return true;
}

bool DatabaseManager::SearchBooks(const std::string& searchTerm, std::vector<Book>& bookList) {
    bookList.clear();
    std::string lower = searchTerm;
    std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);

    for (const auto& b : books) {
        std::string title = b.Title;
        std::string author = b.Author;
        std::string category = b.Category;

        std::transform(title.begin(), title.end(), title.begin(), ::tolower);
        std::transform(author.begin(), author.end(), author.begin(), ::tolower);
        std::transform(category.begin(), category.end(), category.begin(), ::tolower);

        if (title.find(lower) != std::string::npos ||
            author.find(lower) != std::string::npos ||
            category.find(lower) != std::string::npos) {
            bookList.push_back(b);
        }
    }
    return true;
}

bool DatabaseManager::UpdateBook(const Book& book) {
    for (auto& b : books) {
        if (b.BookID == book.BookID) {
            b = book;
            return true;
        }
    }
    return false;
}

bool DatabaseManager::DeleteBook(int id) {
    for (auto it = books.begin(); it != books.end(); ++it) {
        if (it->BookID == id) {
            books.erase(it);
            return true;
        }
    }
    return false;
}

// ===== LOAN OPERATIONS =====
bool DatabaseManager::AddLoan(const Loan& loan) {
    Loan newLoan = loan;
    if (newLoan.LoanID == 0) {
        newLoan.LoanID = (int)loans.size() + 1;
    }
    loans.push_back(newLoan);
    return true;
}

bool DatabaseManager::GetLoanByID(int id, Loan& loan) {
    for (const auto& l : loans) {
        if (l.LoanID == id) {
            loan = l;
            return true;
        }
    }
    return false;
}

bool DatabaseManager::GetLoansByMemberID(int memberId, std::vector<Loan>& loanList) {
    loanList.clear();
    for (const auto& l : loans) {
        if (l.MemberID == memberId) {
            loanList.push_back(l);
        }
    }
    return true;
}

bool DatabaseManager::UpdateLoan(const Loan& loan) {
    for (auto& l : loans) {
        if (l.LoanID == loan.LoanID) {
            l = loan;
            return true;
        }
    }
    return false;
}

bool DatabaseManager::GetActiveLoansByMemberID(int memberId, std::vector<Loan>& loanList) {
    loanList.clear();
    for (const auto& l : loans) {
        if (l.MemberID == memberId && l.Status == "Borrowing") {
            loanList.push_back(l);
        }
    }
    return true;
}

// ===== FINE OPERATIONS =====
bool DatabaseManager::AddFine(const Fine& fine) {
    Fine newFine = fine;
    if (newFine.FineID == 0) {
        newFine.FineID = (int)fines.size() + 1;
    }
    fines.push_back(newFine);
    return true;
}

bool DatabaseManager::GetFinesByLoanID(int loanId, std::vector<Fine>& fineList) {
    fineList.clear();
    for (const auto& f : fines) {
        if (f.LoanID == loanId) {
            fineList.push_back(f);
        }
    }
    return true;
}

bool DatabaseManager::UpdateFineStatus(int fineId, const std::string& status) {
    for (auto& f : fines) {
        if (f.FineID == fineId) {
            f.Status = status;
            return true;
        }
    }
    return false;
}

bool DatabaseManager::GetUnpaidFinesByMemberID(int memberId, std::vector<Fine>& fineList) {
    fineList.clear();

    std::vector<Loan> memberLoans;
    GetLoansByMemberID(memberId, memberLoans);

    for (const auto& f : fines) {
        if (f.Status == "Unpaid") {
            for (const auto& l : memberLoans) {
                if (l.LoanID == f.LoanID) {
                    fineList.push_back(f);
                    break;
                }
            }
        }
    }
    return true;
}

bool DatabaseManager::GetAllFinesByMemberID(int memberId, std::vector<Fine>& fineList) {
    fineList.clear();

    std::vector<Loan> memberLoans;
    GetLoansByMemberID(memberId, memberLoans);

    for (const auto& f : fines) {
        for (const auto& l : memberLoans) {
            if (l.LoanID == f.LoanID) {
                fineList.push_back(f);
                break;
            }
        }
    }
    return true;
}

// ===== NOTIFICATION OPERATIONS =====
bool DatabaseManager::AddNotification(const Notification& notification) {
    Notification newNotif = notification;
    if (newNotif.NotificationID == 0) {
        newNotif.NotificationID = (int)notifications.size() + 1;
    }
    notifications.push_back(newNotif);
    return true;
}

bool DatabaseManager::GetNotificationsByMemberID(int memberId, std::vector<Notification>& notifList) {
    notifList.clear();
    for (const auto& n : notifications) {
        if (n.MemberID == memberId) {
            notifList.push_back(n);
        }
    }
    return true;
}

bool DatabaseManager::UpdateNotificationStatus(int notificationId, const std::string& status) {
    for (auto& n : notifications) {
        if (n.NotificationID == notificationId) {
            n.Status = status;
            return true;
        }
    }
    return false;
}

// ===== AUTHENTICATION =====
bool DatabaseManager::AuthenticateLibrarian(const std::string& username, const std::string& password, Librarian& librarian) {
    for (const auto& lib : librarians) {
        if (lib.Username == username && lib.Password == password) {
            librarian = lib;
            return true;
        }
    }
    return false;
}

bool DatabaseManager::AddLibrarian(const Librarian& librarian) {
    Librarian newLib = librarian;
    if (newLib.LibrarianID == 0) {
        newLib.LibrarianID = (int)librarians.size() + 1;
    }
    librarians.push_back(newLib);
    return true;
}