#include "AuthManager.h"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <functional>
using namespace std;

AuthManager::AuthManager() {
    loadAccounts();
    if (accounts.empty()) {
        addAccount("admin", "admin", Role::ADMIN);
        addAccount("manager", "manager", Role::MANAGER);
        addAccount("employee", "employee", Role::EMPLOYEE);
        saveAccounts();
    }
}

AuthManager::~AuthManager() {
    saveAccounts();
}

void AuthManager::loadAccounts() {
    accounts.clear();
    ifstream f(accountsFile);
    if (!f) return;
    string line;
    while (getline(f, line)) {
        if (line.empty()) continue;
        try {
            accounts.push_back(Account::deserialize(line));
        } catch(...) {
            // ігноруємо биті рядки
        }
    }
}

void AuthManager::saveAccounts() const {
    ofstream f(accountsFile, ios::trunc);
    if (!f) return;
    for (const auto& a : accounts) {
        f << a.serialize() << "\n";
    }
}

optional<pair<string, Role>> AuthManager::authenticateInteractive() {
    string user, pass;
    cout << "==== Log in ====\n";
    cout << "login: ";
    cin >> user;
    cout << "password: ";
    cin >> pass;

    size_t h = hash<string>{}(pass);

    for (const auto& acc : accounts) {
        if (acc.getUsername() == user && acc.getPasswordHash() == h) {
            // Logger::logLogin(user, "SUCCESS as " + Account::roleToString(acc.getRole()));
            cout << "Authorization successful. Congratulations," << user << " (" << Account::roleToString(acc.getRole()) << ")\n";
            return make_pair(user, acc.getRole());
        }
    }
    // Logger::logLogin(user, "FAILED");
    cout << "Incorrect login or password!\n";
    return nullopt;
}

bool AuthManager::addAccount(const string& username, const string& password, Role role) {
    if (accountExists(username)) return false;
    size_t h = hash<string>{}(password);
    accounts.emplace_back(username, h, role);
    saveAccounts();
    // Logger::logAction("System", "Created account " + username + " role=" + Account::roleToString(role));
    return true;
}

bool AuthManager::removeAccount(const string& username) {
    auto it = remove_if(accounts.begin(), accounts.end(), [&](const Account& a){ return a.getUsername() == username; });
    if (it == accounts.end()) return false;
    accounts.erase(it, accounts.end());
    saveAccounts();
    // Logger::logAction("System", "Removed account " + username);
    return true;
}

bool AuthManager::editAccount(const string& username) {
    for (auto& acc : accounts) {
        if (acc.getUsername() == username) {
            cout << "Edit account \"" << username << "\"\n";
            cout << "1. Edit login\n";
            cout << "2. Edit password\n";
            cout << "3. Edit role\n";
            cout << "Choice: ";
            int ch; cin >> ch;

            if (ch == 1) {
                string newName;
                cout << "New login: "; cin >> newName;
                if (accountExists(newName)) {
                    cout << "This login already exists!\n";
                    return false;
                }
                //Logger::logAction("System", "Changed username " + username + " → " + newName);
                acc = Account(newName, acc.getPasswordHash(), acc.getRole());
            } 
            else if (ch == 2) {
                string newPass;
                cout << "New password: "; cin >> newPass;
                size_t h = hash<string>{}(newPass);
                acc = Account(acc.getUsername(), h, acc.getRole());
                //Logger::logAction("System", "Changed password for " + username);
            } 
            else if (ch == 3) {
                int r;
                cout << "New role (0=Admin,1=Manager,2=Employee): "; cin >> r;
                Role newRole = Account::intToRole(r);
                acc = Account(acc.getUsername(), acc.getPasswordHash(), newRole);
                //Logger::logAction("System", "Changed role for " + username + ". New role is " + Account::roleToString(newRole));
            } 
            else {
                cout << "Wrong choice.\n";
                return false;
            }

            saveAccounts();
            cout << "Changes saved.\n";
            return true;
        }
    }
    cout << "Account not found.\n";
    return false;
}

bool AuthManager::accountExists(const string& username) const {
    for (const auto& a : accounts) if (a.getUsername() == username) return true;
    return false;
}

vector<string> AuthManager::listUsernames() const {
    vector<string> res;
    for (const auto& a : accounts) res.push_back(a.getUsername());
    return res;
}
