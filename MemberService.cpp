#include "MemberService.h"
#include <iostream>
using namespace std;

/*
 * Function: registerMember
 * Purpose : Create a new member account (registration use-case)
 * Returns : true if successful, false if email already exists
 */
bool MemberService::registerMember(const string& name, const string& email,
    const string& password, const string& phone,
    const string& expiryDate)
{
    // Prevent duplicate accounts
    if (findByEmail(email) != nullptr) {
        cerr << "[Error] This email already exists. Please use another.\n";
        return false;
    }

    int newID = members.size() + 1; // Simulated auto-increment ID
    Member newMember(newID, name, email, password, phone, "Active", expiryDate);
    members.push_back(newMember);

    cout << "[Success] Member registered successfully: " << name << endl;
    return true;
}

/*
 * Function: login
 * Purpose : Member login with email and password
 * Returns : Member pointer if successful, else nullptr
 */
Member* MemberService::login(const string& email, const string& password)
{
    Member* m = findByEmail(email);
    if (m && m->getPassword() == password) {
        currentMember = m;
        cout << "[Login] Welcome, " << m->getName() << "!\n";
        return m;
    }
    else {
        cerr << "[Login Failed] Invalid email or password.\n";
        return nullptr;
    }
}

/*
 * Function: updateProfile
 * Purpose : Allow logged-in members to update name and phone
 * Returns : true if updated, false if not logged in
 */
bool MemberService::updateProfile(const string& newName, const string& newPhone)
{
    if (!isLoggedIn()) {
        cerr << "[Error] Please log in before updating your profile.\n";
        return false;
    }

    currentMember->setName(newName);
    currentMember->setPhone(newPhone);

    cout << "[Profile Updated Successfully]\n";
    currentMember->displayInfo();
    return true;
}

/*
 * Function: logout
 * Purpose : End the current user session
 */
void MemberService::logout()
{
    if (isLoggedIn()) {
        cout << "[Logout] Goodbye, " << currentMember->getName() << "!\n";
        currentMember = nullptr;
    }
    else {
        cerr << "[Error] No member is currently logged in.\n";
    }
}

/*
 * Function: findByEmail
 * Purpose : Search the mock database for an existing email
 * Returns : Pointer to Member if found, nullptr otherwise
 */
Member* MemberService::findByEmail(const string& email)
{
    for (auto& m : members)
        if (m.getEmail() == email)
            return &m;
    return nullptr;
}
