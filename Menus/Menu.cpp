#include "Menu.h"
#include "AdminMenu.h"
#include "ManagerMenu.h"
#include <iostream>

void Menu::showFor(const string& username, Role role, AuthManager& auth, ZooGraph& zoo) {
    switch (role) {
        case Role::ADMIN:
            AdminMenu::show(username, auth, zoo);
        break;
        case Role::MANAGER:
            ManagerMenu::show(username, auth, zoo);
        break;
        case Role::EMPLOYEE:
        {
            bool running = true;
            while (running) {
                cout << "\n--- Employee Menu (logged as " << username << ") ---\n";
                cout << "1. View your tasks\n";
                cout << "2. View animals\n";
                cout << "3.  Return to login screen\n";
                cout << "Choice: ";
                int ch; cin >> ch;
                switch (ch) {
                    case 1:
                    //    cout << "Ваші завдання: годування, прибирання, огляд тварин.\n";
                    //Logger::logAction(username, "Viewed tasks");
                    break;
                    case 2:
                        zoo.listAnimals();
                        //Logger::logAction(username, "Viewed animals");
                    break;
                    case 3:
                        running = false;
                    break;
                    default:
                        cout << "Wrong choice\n";
                }
            }
        }
        break;
        default:
            break;
    }
}
