#include "AuthManager.h"
#include "../Logger/LoggerGlobal.h"
#include "../DatabaseManager/AccountRepository.h"
#include <iostream>
#include <functional>
#include <algorithm>

using namespace std;

AuthManager::AuthManager(AccountRepository& repository)
    : repo(repository) {
    logger.debug("Initializing AuthManager with database-backed repository...");
    repo.initTable();
   //db.debugListTables()

    auto all = repo.getAllAccounts();
    if (all.empty()) {
        logger.warn("No accounts found in DB. Creating default accounts...");
        addAccount("admin", "admin", Role::ADMIN);
        addAccount("manager", "manager", Role::MANAGER);
        addAccount("employee", "employee", Role::EMPLOYEE);
        logger.info("Default accounts created in database.");
    } else {
        logger.info("Loaded " + to_string(all.size()) + " accounts from database.");
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
    logger.debug("Attempting login for user: " + user);

    auto accOpt = repo.getAccount(user);
    if (accOpt.has_value()) {
        const Account& acc = accOpt.value();
        if (acc.getPasswordHash() == h) {
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
    bool ok = repo.addAccount(username, h, role);
    if (ok)
        logger.info("Created new account: " + username + " [role=" + Account::roleToString(role) + "]");
    else
        logger.error("Failed to add account: " + username);
    return ok;
}

bool AuthManager::removeAccount(const string& username) {
    logger.debug("Attempting to remove account: " + username);
    if (!accountExists(username)) {
        logger.warn("Account not found: " + username);
        return false;
    }

    bool ok = repo.removeAccount(username);
    if (ok)
        logger.info("Account removed: " + username);
    else
        logger.error("Failed to remove account: " + username);
    return ok;
}

bool AuthManager::editAccount(const string& username) {
    logger.debug("Editing account: " + username);

    auto accOpt = repo.getAccount(username);
    if (!accOpt.has_value()) {
        cout << "Account not found.\n";
        logger.warn("Edit failed: account not found (" + username + ")");
        return false;
    }

    auto acc = accOpt.value();
    cout << "Edit account \"" << username << "\"\n";
    cout << "1. Edit login\n";
    cout << "2. Edit password\n";
    cout << "3. Edit role\n";
    cout << "Choice: ";
    int ch;
    cin >> ch;

    string newUsername = username;
    optional<size_t> newHash = nullopt;
    optional<Role> newRole = nullopt;

    if (ch == 1) {
        cout << "New login: ";
        cin >> newUsername;
        if (accountExists(newUsername)) {
            cout << "This login already exists!\n";
            logger.warn("Attempted rename to existing username: " + newUsername);
            return false;
        }
    } else if (ch == 2) {
        string newPass;
        cout << "New password: ";
        cin >> newPass;
        newHash = hash<string>{}(newPass);
    } else if (ch == 3) {
        int r;
        cout << "New role (0=Admin, 1=Manager, 2=Employee): ";
        cin >> r;
        try {
            newRole = Account::intToRole(r);
        } catch (...) {
            cout << "Invalid role.\n";
            logger.warn("Invalid role entered for user: " + username);
            return false;
        }
    } else {
        cout << "Wrong choice.\n";
        logger.warn("Invalid menu choice for account editing.");
        return false;
    }

    bool ok = repo.updateAccount(username, newUsername, newHash, newRole);
    if (ok)
        logger.info("Account updated: " + username + " → " + newUsername);
    else
        logger.error("Failed to update account: " + username);
    return ok;
}

bool AuthManager::accountExists(const string& username) const {
    return repo.accountExists(username);
}

vector<string> AuthManager::listUsernames() const {
    logger.debug("Listing all usernames from DB.");
    vector<string> result;
    for (const auto& acc : repo.getAllAccounts())
        result.push_back(acc.getUsername());
    logger.info("Listed " + to_string(result.size()) + " usernames.");
    return result;
}
