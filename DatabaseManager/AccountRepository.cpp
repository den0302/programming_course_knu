#include "AccountRepository.h"
#include "../Logger/LoggerGlobal.h"
#include "SQLUtilities/SQLUtils.h"
#include <sstream>
using namespace std;

AccountRepository::AccountRepository(DatabaseManager& database) : db(database) {}

void AccountRepository::initTable() {
    string sql =
        "CREATE TABLE IF NOT EXISTS accounts ("
        "username TEXT PRIMARY KEY, "
        "passwordHash TEXT NOT NULL, "
        "role INTEGER NOT NULL"
        ");";
    if (!db.getDB()) {
        logger.error("Database pointer is null before initTable!");
        return;
    }
    const char* dbFile = sqlite3_db_filename(db.getDB(), "main");
    logger.debug(string("AccountRepository::initTable - DB file: ") + (dbFile ? dbFile : "unknown"));

    if (!db.execute(sql)) {
        logger.error("Failed to create table 'accounts'. See previous SQL error.");
        return;
    }
    logger.info("Table 'accounts' ensured.");
}

bool AccountRepository::addAccount(const string& username, size_t passwordHash, Role role) {
    string safeUsername = escapeSQL(username);
    string sql =
        "INSERT INTO accounts (username, passwordHash, role) VALUES ('" + safeUsername + "', '" +
        to_string(passwordHash) + "', '" + to_string(Account::roleToInt(role)) + "');";

    bool ok = db.execute(sql);
    if (ok) logger.info("Account added: " + username);
    else logger.error("Failed to insert account: " + username);
    return ok;
}

bool AccountRepository::removeAccount(const string& username) {
    string sql = "DELETE FROM accounts WHERE username='" + username + "';";
    bool ok = db.execute(sql);
    if (ok) logger.info("Account removed: " + username);
    else logger.warn("Failed to remove account: " + username);
    return ok;
}

bool AccountRepository::updateAccount(const string& username,
                                      const string& newUsername,
                                      optional<size_t> newPasswordHash,
                                      optional<Role> newRole) {
    ostringstream oss;
    oss << "UPDATE accounts SET username='" << newUsername << "'";

    if (newPasswordHash.has_value())
        oss << ", passwordHash='" << to_string(*newPasswordHash) << "'";

    if (newRole.has_value())
        oss << ", role='" << to_string(Account::roleToInt(*newRole)) << "'";

    oss << " WHERE username='" << username << "';";

    bool ok = db.execute(oss.str());
    if (ok) logger.info("Account updated: " + username);
    else logger.error("Failed to update account: " + username);
    return ok;
}

optional<Account> AccountRepository::getAccount(const string& username) {
    string sql = "SELECT username, passwordHash, role FROM accounts WHERE username='" + username + "';";
    sqlite3_stmt* stmt;
    if (!db.prepare(sql, &stmt)) {
        logger.error("Failed to prepare SELECT for account: " + username);
        return nullopt;
    }

    optional<Account> result;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        string u = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        size_t hash = static_cast<size_t>(sqlite3_column_int64(stmt, 1));
        Role role = Account::intToRole(sqlite3_column_int(stmt, 2));
        result = Account(u, hash, role);
    }
    sqlite3_finalize(stmt);
    return result;
}

vector<Account> AccountRepository::getAllAccounts() {
    vector<Account> res;
    string sql = "SELECT username, passwordHash, role FROM accounts;";
    sqlite3_stmt* stmt;

    if (!db.prepare(sql, &stmt)) {
        logger.error("Failed to prepare SELECT for all accounts.");
        return res;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        string u = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        size_t hash = static_cast<size_t>(sqlite3_column_int64(stmt, 1));
        Role role = Account::intToRole(sqlite3_column_int(stmt, 2));
        res.emplace_back(u, hash, role);
    }

    sqlite3_finalize(stmt);
    return res;
}

bool AccountRepository::accountExists(const string& username) {
    string sql = "SELECT COUNT(*) FROM accounts WHERE username='" + username + "';";
    sqlite3_stmt* stmt;
    if (!db.prepare(sql, &stmt)) return false;

    bool exists = false;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        exists = sqlite3_column_int(stmt, 0) > 0;
    }
    sqlite3_finalize(stmt);
    return exists;
}
