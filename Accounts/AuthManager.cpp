#include "AuthManager.h"
#include "../Logger/LoggerGlobal.h"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <functional>

using namespace std;

AuthManager::AuthManager() {
    logger.debug("Initializing AuthManager...");
    loadAccounts();

    if (accounts.empty()) {
        logger.warn("No accounts found. Creating default ones.");
        addAccount("admin", "admin", Role::ADMIN);
        addAccount("manager", "manager", Role::MANAGER);
        addAccount("employee", "employee", Role::EMPLOYEE);
        saveAccounts();
        logger.info("Default accounts created and saved.");
    } else {
        logger.info("Accounts loaded successfully (" + to_string(accounts.size()) + " found).");
    }
}

AuthManager::~AuthManager() {
    logger.debug("Saving accounts before shutdown...");
    saveAccounts();
}

void AuthManager::loadAccounts() {
    logger.debug("Loading accounts from file: " + accountsFile);
    accounts.clear();
    ifstream f(accountsFile);
    if (!f) {
        logger.warn("Accounts file not found: " + accountsFile);
        return;
    }

    string line;
    while (getline(f, line)) {
        if (line.empty()) continue;
        try {
            accounts.push_back(Account::deserialize(line));
        } catch (...) {
            logger.error("Failed to deserialize account line: " + line);
        }
    }

    logger.info("Loaded " + to_string(accounts.size()) + " accounts from file.");
}

void AuthManager::saveAccounts() const {
    logger.debug("Saving accounts to file: " + accountsFile);
    ofstream f(accountsFile, ios::trunc);
    if (!f) {
        logger.error("Failed to open file for saving accounts: " + accountsFile);
        return;
    }
    for (const auto& a : accounts) {
        f << a.serialize() << "\n";
    }
    logger.info("Accounts successfully saved (" + to_string(accounts.size()) + " records).");
}

optional<pair<string, Role>> AuthManager::authenticateInteractive() {
    string user, pass;
    cout << "==== Log in ====\n";
    cout << "login: ";
    cin >> user;
    cout << "password: ";
    cin >> pass;

    size_t h = hash<string>{}(pass);
    logger.debug("User attempting login: " + user);

    for (const auto& acc : accounts) {
        if (acc.getUsername() == user && acc.getPasswordHash() == h) {
            logger.info("Login SUCCESS: " + user + " (" + Account::roleToString(acc.getRole()) + ")");
            cout << "Authorization successful. Welcome, " << user
                 << " (" << Account::roleToString(acc.getRole()) << ")\n";
            return make_pair(user, acc.getRole());
        }
    }

    logger.warn("Login FAILED for user: " + user);
    cout << "Incorrect login or password!\n";
    return nullopt;
}

bool AuthManager::addAccount(const string& username, const string& password, Role role) {
    logger.debug("Attempting to add account: " + username);
    if (accountExists(username)) {
        logger.warn("Account creation failed: username already exists (" + username + ")");
        return false;
    }

    size_t h = hash<string>{}(password);
    accounts.emplace_back(username, h, role);
    saveAccounts();

    logger.info("Created new account: " + username + " [role=" + Account::roleToString(role) + "]");
    return true;
}

bool AuthManager::removeAccount(const string& username) {
    logger.debug("Attempting to remove account: " + username);
    auto it = remove_if(accounts.begin(), accounts.end(),
                        [&](const Account& a) { return a.getUsername() == username; });

    if (it == accounts.end()) {
        logger.warn("Account not found for removal: " + username);
        return false;
    }

    accounts.erase(it, accounts.end());
    saveAccounts();

    logger.info("Removed account: " + username);
    return true;
}

bool AuthManager::editAccount(const string& username) {
    logger.debug("Editing account: " + username);

    for (auto& acc : accounts) {
        if (acc.getUsername() == username) {
            cout << "Edit account \"" << username << "\"\n";
            cout << "1. Edit login\n";
            cout << "2. Edit password\n";
            cout << "3. Edit role\n";
            cout << "Choice: ";
            int ch;
            cin >> ch;

            if (ch == 1) {
                string newName;
                cout << "New login: ";
                cin >> newName;

                if (accountExists(newName)) {
                    logger.warn("Attempted to rename to existing username: " + newName);
                    cout << "This login already exists!\n";
                    return false;
                }

                logger.info("Changed username: " + username + " → " + newName);
                acc = Account(newName, acc.getPasswordHash(), acc.getRole());
            } 
            else if (ch == 2) {
                string newPass;
                cout << "New password: ";
                cin >> newPass;
                size_t h = hash<string>{}(newPass);
                acc = Account(acc.getUsername(), h, acc.getRole());
                logger.info("Changed password for user: " + username);
            } 
            else if (ch == 3) {
                int r;
                cout << "New role (0=Admin,1=Manager,2=Employee): ";
                cin >> r;
                Role newRole = Account::intToRole(r);
                acc = Account(acc.getUsername(), acc.getPasswordHash(), newRole);
                logger.info("Changed role for user " + username + " → " + Account::roleToString(newRole));
            } 
            else {
                logger.warn("Invalid edit option selected for user: " + username);
                cout << "Wrong choice.\n";
                return false;
            }

            saveAccounts();
            logger.info("Account changes saved for: " + username);
            cout << "Changes saved.\n";
            return true;
        }
    }

    logger.warn("Edit failed: account not found (" + username + ")");
    cout << "Account not found.\n";
    return false;
}

bool AuthManager::accountExists(const string& username) const {
    for (const auto& a : accounts)
        if (a.getUsername() == username)
            return true;
    return false;
}

vector<string> AuthManager::listUsernames() const {
    logger.debug("Listing all usernames.");
    vector<string> res;
    for (const auto& a : accounts)
        res.push_back(a.getUsername());
    logger.info("Listed " + to_string(res.size()) + " usernames.");
    return res;
}
