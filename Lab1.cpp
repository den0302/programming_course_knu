#include <string>
#include <iostream>
#include "Logger/LoggerGlobal.h"
#include "Graphs/ZooGraph.h"
#include "Menus/Menu.h"
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

    AuthManager auth;
    ZooGraph zoo;

    while (true) {
        auto res = auth.authenticateInteractive();
        if (!res.has_value()) {
            cout << "Try again? (y/n): ";
            char c; cin >> c;
            if (c == 'y' || c == 'Y') continue;
            break; 
        }

        string username = res->first;
        Role role = res->second;

        Menu::showFor(username, role, auth, zoo );

        cout << "Exit the program or return to login? (q - exit, any other key - login): ";
        char c; cin >> c;
        if (c == 'q' || c == 'Q') break;
    }
    cout << "Bye!\n";
    return 0;
}
