#ifndef ACCOUNTREPOSITORY_H
#define ACCOUNTREPOSITORY_H

#include "DatabaseManager.h"
#include <string>
#include <vector>
#include <optional>
#include "../Accounts/AuthManager.h"

using namespace std;

class AccountRepository {
private:
    DatabaseManager& db;

public:
    explicit AccountRepository(DatabaseManager& database);

    void initTable();

    bool addAccount(const string& username, size_t passwordHash, Role role);
    bool removeAccount(const string& username);
    bool updateAccount(const string& username, const string& newUsername,
                       optional<size_t> newPasswordHash, optional<Role> newRole);

    optional<Account> getAccount(const string& username);
    vector<Account> getAllAccounts();
    bool accountExists(const string& username);
};

#endif // ACCOUNTREPOSITORY_H
