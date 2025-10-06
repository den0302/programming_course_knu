#include "ManagerMenu.h"
#include <iostream>
using namespace std;

void ManagerMenu::show(const string& username, AuthManager& auth, ZooGraph& zoo) {
    bool running = true;
    while (running) {
        cout << "\n--- Manager Menu (logged as " << username << ") ---\n";
        cout << "1. Assign worker to aviary\n";
        cout << "2. View list of workers\n";
        cout << "3. Return to login screen\n";
        cout << "Choice: ";
        int ch; cin >> ch;

        switch (ch) {
            case 1: {
                string empId, aviaryId;
                cout << "ID employee: "; cin >> empId;
                cout << "ID aviary: "; cin >> aviaryId;
                zoo.getEmployeeManager().assignEmployeeToAviary(empId, aviaryId);
                cout << "Призначено.\n";
                // Logger::logAction(username, "Assigned employee " + to_string(empId) + " to aviary " + to_string(aviaryId));
                break;
            }
            case 2: {
                zoo.listEmployees();
                break;
            }
            case 3:
                running = false;
            break;
            default:
                cout << "Wrong choice\n";
        }
    }
}
