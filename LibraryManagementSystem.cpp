#include <iostream>
using namespace std;

int main() {
    int choice;

    do {
        cout << "\n===== LIBRARY SYSTEM =====\n";
        cout << "1. Login\n";
        cout << "2. Search Book\n";
        cout << "3. Issue Book\n";
        cout << "4. Return Book\n";
        cout << "5. Exit\n";
        cout << "Choose: ";
        cin >> choice;

        switch (choice) {
        case 1:
            cout << "Login...\n";
            break;
        case 2:
            cout << "Search Book...\n";
            break;
        case 3:
            cout << "Issue Book...\n";
            break;
        case 4:
            cout << "Return Book...\n";
            break;
        }

    } while (choice != 5);

    return 0;
}