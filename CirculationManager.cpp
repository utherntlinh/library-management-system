#define _CRT_SECURE_NO_WARNINGS
#include "CirculationManager.h"
#include <iostream>
#include <ctime>
#include <iomanip>
#include <sstream>

CirculationManager::CirculationManager(std::shared_ptr<DatabaseManager> db)
    : dbManager(db) {
}

CirculationManager::~CirculationManager() {}

std::string CirculationManager::GetCurrentDate() {
    time_t now = time(nullptr);
    tm* timeinfo = localtime(&now);

    std::ostringstream dateStream;
    dateStream << std::put_time(timeinfo, "%Y-%m-%d");
    return dateStream.str();
}

std::string CirculationManager::GetDueDate() {
    time_t now = time(nullptr);
    tm* timeinfo = localtime(&now);
    timeinfo->tm_mday += LOAN_DAYS;
    mktime(timeinfo);

    std::ostringstream dateStream;
    dateStream << std::put_time(timeinfo, "%Y-%m-%d");
    return dateStream.str();
}

bool CirculationManager::CheckBookAvailability(int bookId) {
    Book book;
    if (!dbManager->GetBookByID(bookId, book)) {
        return false;
    }

    return book.AvailableQuantity > 0;
}

bool CirculationManager::CheckUnpaidFines(int memberId) {
    std::vector<Fine> unpaidFines;
    dbManager->GetUnpaidFinesByMemberID(memberId, unpaidFines);
    return unpaidFines.empty();
}

bool CirculationManager::IssueBook(int memberId, int bookId, std::string& errorMsg) {
    // Step 1: Check if member exists and is active
    Member member;
    if (!dbManager->GetMemberByID(memberId, member)) {
        errorMsg = "Member not found!";
        return false;
    }

    if (member.Status != "Active") {
        errorMsg = "Member account is " + member.Status + "!";
        return false;
    }

    // Step 2: Check book availability (Mandatory)
    if (!CheckBookAvailability(bookId)) {
        errorMsg = "Book is not available!";
        return false;
    }

    // Step 3: Check unpaid fines (Mandatory)
    if (!CheckUnpaidFines(memberId)) {
        double totalFines = GetTotalUnpaidFines(memberId);
        std::ostringstream oss;
        oss << "Member has unpaid fines: " << totalFines << ". Please settle before borrowing.";
        errorMsg = oss.str();
        return false;
    }

    // Step 4: Create loan record
    Loan newLoan;
    newLoan.MemberID = memberId;
    newLoan.BookID = bookId;
    newLoan.BorrowDate = GetCurrentDate();
    newLoan.DueDate = GetDueDate();
    newLoan.Status = "Borrowing";

    if (!dbManager->AddLoan(newLoan)) {
        errorMsg = "Failed to create loan record!";
        return false;
    }

    // Step 5: Update book availability
    Book book;
    if (!dbManager->GetBookByID(bookId, book)) {
        errorMsg = "Failed to update book availability!";
        return false;
    }

    book.AvailableQuantity--;
    if (!dbManager->UpdateBook(book)) {
        errorMsg = "Failed to update book availability!";
        return false;
    }

    // Step 6: Send notification
    Notification notification;
    notification.MemberID = memberId;
    notification.Message = "You have successfully borrowed: " + book.Title;
    notification.Date = GetCurrentDate();
    notification.Status = "Sent";
    dbManager->AddNotification(notification);

    std::cout << "Book issued successfully!" << std::endl;
    std::cout << "Borrow Date: " << newLoan.BorrowDate << std::endl;
    std::cout << "Due Date: " << newLoan.DueDate << std::endl;

    return true;
}

bool CirculationManager::ReturnBook(int loanId, std::string& errorMsg) {
    // Get loan details
    Loan loan;
    if (!dbManager->GetLoanByID(loanId, loan)) {
        errorMsg = "Loan record not found!";
        return false;
    }

    if (loan.Status != "Borrowing") {
        errorMsg = "This book has already been returned!";
        return false;
    }

    // Get book details
    Book book;
    if (!dbManager->GetBookByID(loan.BookID, book)) {
        errorMsg = "Book not found!";
        return false;
    }

    // Get member details
    Member member;
    if (!dbManager->GetMemberByID(loan.MemberID, member)) {
        errorMsg = "Member not found!";
        return false;
    }

    // Update loan status
    loan.ReturnDate = GetCurrentDate();
    loan.Status = "Returned";

    if (!dbManager->UpdateLoan(loan)) {
        errorMsg = "Failed to update loan status!";
        return false;
    }

    // Update book availability
    book.AvailableQuantity++;
    if (!dbManager->UpdateBook(book)) {
        errorMsg = "Failed to update book availability!";
        return false;
    }

    // Check if overdue and calculate fine
    double fine = CalculateFine(loan.DueDate);
    if (fine > 0) {
        Fine overdueFine;
        overdueFine.LoanID = loanId;
        overdueFine.Amount = fine;
        overdueFine.Status = "Unpaid";

        if (!dbManager->AddFine(overdueFine)) {
            std::cout << "Warning: Failed to record fine!" << std::endl;
        }

        // Send notification about fine
        Notification fineNotification;
        fineNotification.MemberID = loan.MemberID;
        std::ostringstream msg;
        msg << "You have an overdue fine of " << fine << " for: " << book.Title;
        fineNotification.Message = msg.str();
        fineNotification.Date = GetCurrentDate();
        fineNotification.Status = "Sent";
        dbManager->AddNotification(fineNotification);

        std::cout << "Book returned. Overdue fine: " << fine << std::endl;
    }
    else {
        // Send notification
        Notification returnNotification;
        returnNotification.MemberID = loan.MemberID;
        returnNotification.Message = "You have successfully returned: " + book.Title;
        returnNotification.Date = GetCurrentDate();
        returnNotification.Status = "Sent";
        dbManager->AddNotification(returnNotification);

        std::cout << "Book returned successfully!" << std::endl;
    }

    return true;
}

double CirculationManager::CalculateFine(const std::string& dueDate) {
    std::string currentDate = GetCurrentDate();
    int daysDifference = GetDaysDifference(dueDate, currentDate);

    if (daysDifference > 0) {
        return daysDifference * FINE_PER_DAY;
    }

    return 0.0;
}

int CirculationManager::GetDaysDifference(const std::string& date1, const std::string& date2) {
    std::tm tm1 = {};
    std::tm tm2 = {};

    std::istringstream ss1(date1);
    ss1 >> std::get_time(&tm1, "%Y-%m-%d");

    std::istringstream ss2(date2);
    ss2 >> std::get_time(&tm2, "%Y-%m-%d");

    time_t time1 = std::mktime(&tm1);
    time_t time2 = std::mktime(&tm2);

    return static_cast<int>((time2 - time1) / (60 * 60 * 24));
}

bool CirculationManager::GetMemberLoans(int memberId, std::vector<Loan>& loans) {
    return dbManager->GetLoansByMemberID(memberId, loans);
}

double CirculationManager::GetTotalUnpaidFines(int memberId) {
    std::vector<Fine> unpaidFines;
    dbManager->GetUnpaidFinesByMemberID(memberId, unpaidFines);

    double total = 0.0;
    for (const auto& fine : unpaidFines) {
        total += fine.Amount;
    }

    return total;
}

double CirculationManager::GetTotalPaidFines(int memberId) {
    std::vector<Fine> allFines;
    dbManager->GetAllFinesByMemberID(memberId, allFines);

    double total = 0.0;
    for (const auto& fine : allFines) {
        if (fine.Status == "Paid") {
            total += fine.Amount;
        }
    }
    return total;
}