#include "AdminMenu.h"
#include <iostream>
using namespace std;

void AdminMenu::show(const string& username, AuthManager& auth, ZooGraph& zoo) {
    bool running = true;
    while (running) {
        cout << "\n--- Admin Menu (logged as " << username << ") ---\n";
        cout << "1. Create account\n";
        cout << "2. delete account\n";
        cout << "3. Show list of all accounts\n";
        cout << "4. View action log\n";
        cout << "5. Return to the login screen\n";
        cout << "6. Edit account\n";
        cout << "Choice: ";
        int ch; cin >> ch;

        switch (ch) {
            case 1: {
                string u, p; int r;
                cout << "New login: "; cin >> u;
                cout << "Password: "; cin >> p;
                cout << "Role (0=Admin,1=Manager,2=Employee): "; cin >> r;
                Role role = Account::intToRole(r);
                if (auth.addAccount(u, p, role)) {
                    cout << "Account added.\n";
                    // Logger::logAction(username, "Added account " + u);
                } else {
                    cout << "An account with that name already exists.\n";
                }
                break;
            }
            case 2: {
                string u;
                cout << "Login to delete: "; cin >> u;
                if (auth.removeAccount(u)) {
                    cout << "Account deleted.\n";
                    // Logger::logAction(username, "Removed account " + u);
                } else {
                    cout << "Account not found.\n";
                }
                break;
            }
            case 3: {
                auto names = auth.listUsernames();
                cout << "Account in system:\n";
                for (auto &n : names) cout << " - " << n << "\n";
                break;
            }
            case 4: {
                // ifstream f("log.txt");
                // if (!f) { cout << "Лог не знайдено.\n"; break; }
                // cout << f.rdbuf();
                break;
            }
            case 5:
                running = false;
                break;
            case 6: {
                string u;
                cout << "Enter account login to edit: ";
                cin >> u;
                auth.editAccount(u);
                break;
            }
            default:
                cout << "Wrong choice\n";
        }
    }
}
