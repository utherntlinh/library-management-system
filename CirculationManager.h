#ifndef CIRCULATION_MANAGER_H
#define CIRCULATION_MANAGER_H

#include "DatabaseManager.h"
#include "Models.h"
#include <memory>
#include <vector>

class CirculationManager {
private:
    std::shared_ptr<DatabaseManager> dbManager;
    const int LOAN_DAYS = 14;
    const double FINE_PER_DAY = 10.0;

public:
    CirculationManager(std::shared_ptr<DatabaseManager> db);
    ~CirculationManager();

    // Issue book
    bool IssueBook(int memberId, int bookId, std::string& errorMsg);

    // Return book
    bool ReturnBook(int loanId, std::string& errorMsg);

    // Check book availability
    bool CheckBookAvailability(int bookId);

    // Check unpaid fines
    bool CheckUnpaidFines(int memberId);

    // Get member's active loans
    bool GetMemberLoans(int memberId, std::vector<Loan>& loans);

    // Calculate fine for overdue book
    double CalculateFine(const std::string& dueDate);

    // Get total unpaid fines
    double GetTotalUnpaidFines(int memberId);

    // Get total paid fines
    double GetTotalPaidFines(int memberId);

private:
    std::string GetCurrentDate();
    std::string GetDueDate();
    int GetDaysDifference(const std::string& date1, const std::string& date2);
};

#endif // CIRCULATION_MANAGER_H