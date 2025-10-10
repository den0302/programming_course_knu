#include <string>
#include <iostream>
#include "Logger/LoggerGlobal.h"
#include "Graphs/ZooGraph.h"
#include "Menus/Menu.h"
#include "DatabaseManager/AccountRepository.h"
#include "DatabaseManager/AnimalRepository.h"
#include "DatabaseManager/EmployeeRepository.h"
#ifdef _WIN32
#include <windows.h>
void enableANSI() {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);
}
#endif

using namespace std;

int main() {

    #ifdef _WIN32
        enableANSI();
    #endif

    DatabaseManager db("zoo.db");
    AccountRepository accRepo(db);
    AuthManager auth(accRepo);
    AviaryRepository aviaryRepo(db);
    PathRepository pathRepo(db);
    AnimalRepository animalRepo(db);
    EmployeeRepository employeeRepo(db);
    ZooGraph zoo(aviaryRepo, pathRepo, animalRepo, employeeRepo);

    while (true) {
        auto session = auth.authenticateInteractive();
        if (!session.has_value()) {
            cout << "Try again? (y/n): ";
            char c; cin >> c;
            if (c == 'y' || c == 'Y') continue;
            break; 
        }

        string username = session->first;
        Role role = session->second;

        Menu::showFor(username, role, auth, zoo );

        cout << "Exit the program or return to login? (q - exit, any other key - login): ";
        char c; cin >> c;
        if (c == 'q' || c == 'Q') break;
    }
    cout << "Bye!\n";
    return 0;
}
