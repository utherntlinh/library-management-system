#include "DatabaseManager.h"
#include "AuthenticationManager.h"
#include <iostream>

// Insert Test Data 
void InsertTestData(std::shared_ptr<DatabaseManager> dbManager) {
    std::vector<Book> existingBooks;
    dbManager->GetAllBooks(existingBooks);
    if (!existingBooks.empty()) {
        std::cout << "[*] Test data already loaded, skipping." << std::endl;
        return;
    }

    std::cout << "\n[*] Inserting test data...\n" << std::endl;

    // ===== BOOKS =====
    Book book1;
    book1.BookID = 1;
    book1.ISBN = "001";
    book1.Title = "Harry Potter";
    book1.Author = "J.K. Rowling";
    book1.Category = "Fantasy";
    book1.Publisher = "Bloomsbury";
    book1.Year = 1997;
    book1.Quantity = 10;
    book1.AvailableQuantity = 10;
    dbManager->AddBook(book1);
    std::cout << "[+] Added book: " << book1.Title << std::endl;

    Book book2;
    book2.BookID = 2;
    book2.ISBN = "002";
    book2.Title = "Clean Code";
    book2.Author = "Robert C. Martin";
    book2.Category = "IT";
    book2.Publisher = "Prentice Hall";
    book2.Year = 2008;
    book2.Quantity = 5;
    book2.AvailableQuantity = 5;
    dbManager->AddBook(book2);
    std::cout << "[+] Added book: " << book2.Title << std::endl;

    Book book3;
    book3.BookID = 3;
    book3.ISBN = "003";
    book3.Title = "The Alchemist";
    book3.Author = "Paulo Coelho";
    book3.Category = "Fiction";
    book3.Publisher = "HarperOne";
    book3.Year = 1988;
    book3.Quantity = 7;
    book3.AvailableQuantity = 7;
    dbManager->AddBook(book3);
    std::cout << "[+] Added book: " << book3.Title << std::endl;

    // ===== MEMBERS =====
    auto authManager = std::make_shared<AuthenticationManager>(dbManager);

    authManager->RegisterNewMember("Huy", "huy@email.com", "0977", "huy123");
    std::cout << "[+] Added member: Huy (huy@email.com)" << std::endl;

    authManager->RegisterNewMember("Hung", "hung@email.com", "0376", "hung123");
    std::cout << "[+] Added member: Hung (hung@email.com)" << std::endl;

    authManager->RegisterNewMember("Linh", "linh@email.com", "0473", "linh123");
    std::cout << "[+] Added member: Linh (linh@email.com)" << std::endl;

    // ===== LOANS =====
    Member huy, hung, linh;
    dbManager->GetMemberByEmail("huy@email.com", huy);
    dbManager->GetMemberByEmail("hung@email.com", hung);
    dbManager->GetMemberByEmail("linh@email.com", linh);

    Loan loan1;
    loan1.MemberID = huy.MemberID;
    loan1.BookID = 1;
    loan1.BorrowDate = "2026-04-01";
    loan1.DueDate = "2026-04-15";
    loan1.ReturnDate = "";
    loan1.Status = "Borrowing";
    dbManager->AddLoan(loan1);
    std::cout << "[+] Added loan: Huy borrowed book 1" << std::endl;

    Loan loan2;
    loan2.MemberID = hung.MemberID;
    loan2.BookID = 2;
    loan2.BorrowDate = "2026-04-05";
    loan2.DueDate = "2026-04-19";
    loan2.ReturnDate = "";
    loan2.Status = "Borrowing";
    dbManager->AddLoan(loan2);
    std::cout << "[+] Added loan: Hung borrowed book 2" << std::endl;

    std::vector<Loan> hungLoans;
    dbManager->GetLoansByMemberID(hung.MemberID, hungLoans);
    std::vector<Loan> linhLoans;
    dbManager->GetLoansByMemberID(linh.MemberID, linhLoans);

    // ===== FINES =====
    if (!hungLoans.empty()) {
        Fine hungFine;
        hungFine.LoanID = hungLoans[0].LoanID;  
        hungFine.Amount = 100000;
        hungFine.Status = "Unpaid";
        dbManager->AddFine(hungFine);
        std::cout << "[+] Added fine for Hung: 100000 VND (Unpaid)" << std::endl;
    }

    Loan linhOldLoan;
    linhOldLoan.MemberID = linh.MemberID;
    linhOldLoan.BookID = 3;
    linhOldLoan.BorrowDate = "2026-03-01";
    linhOldLoan.DueDate = "2026-03-15";
    linhOldLoan.ReturnDate = "2026-03-20";
    linhOldLoan.Status = "Returned";
    dbManager->AddLoan(linhOldLoan);

    dbManager->GetLoansByMemberID(linh.MemberID, linhLoans);
    if (!linhLoans.empty()) {
        Fine linhFine;
        linhFine.LoanID = linhLoans[0].LoanID;  
        linhFine.Amount = 150000;
        linhFine.Status = "Paid";
        dbManager->AddFine(linhFine);
        std::cout << "[+] Added fine for Linh: 150000 VND (Paid)" << std::endl;
    }

    // ===== NOTIFICATIONS =====
    Notification notif1;
    notif1.NotificationID = 3001;
    notif1.MemberID = huy.MemberID;
    notif1.Message = "Your book is due on 2026-04-15. Please return on time!";
    notif1.Date = "2026-04-10";
    notif1.Status = "Sent";
    dbManager->AddNotification(notif1);
    std::cout << "[+] Added notification for Huy" << std::endl;

    std::cout << "\n[OK] Test data inserted successfully!\n" << std::endl;
    std::cout << "[*] Test Accounts:" << std::endl;
    std::cout << "  Member  - huy@email.com  / huy123" << std::endl;
    std::cout << "  Member  - hung@email.com / hung123" << std::endl;
    std::cout << "  Member  - linh@email.com / linh123" << std::endl;
    std::cout << "  Librarian - admin / admin123" << std::endl;
}