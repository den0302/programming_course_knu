#include "DatabaseManager.h"
#include "../Logger/LoggerGlobal.h"

DatabaseManager::DatabaseManager(const string& name) : db(nullptr), dbName(name) {
    const char*  createAccountsTableSQL = R"(
CREATE TABLE IF NOT EXISTS accounts (
    username TEXT PRIMARY KEY,
    passwordHash TEXT NOT NULL,
    role INTEGER NOT NULL
);
)";
    char* errMsg = nullptr;
    if (sqlite3_exec(db, createAccountsTableSQL, nullptr, nullptr, &errMsg) != SQLITE_OK) {
        logger.error("SQL error: " + std::string(errMsg ? errMsg : "unknown"));
        sqlite3_free(errMsg);
    } else {
        logger.info("Tablee 'accounts' ensured.");
    }
    logger.info("sql :\n");
    logger.info(createAccountsTableSQL);
    //logger.info("db : " + std::to_string(db.getDB()));

}

bool DatabaseManager::open() {
    int rc = sqlite3_open(dbName.c_str(), &db);
    if (rc != SQLITE_OK) {
        logger.error("Failed to open database: " + std::string(sqlite3_errmsg(db)));
    } else {
        logger.debug("Database opened successfully: " + std::string(dbName.c_str()));
    }
    if (rc) {
        cerr << "Failed to open database: " << sqlite3_errmsg(db) << endl;
        logger.error("Failed to open database: " + std::string(sqlite3_errmsg(db)));
        return false;
    }

    return true;

}

void DatabaseManager::close() {
    if (db) sqlite3_close(db);
    db = nullptr;
}
bool DatabaseManager::execute(const string& query) {
    char* errMsg = nullptr;
    int rc = sqlite3_exec(db, query.c_str(), nullptr, nullptr, &errMsg);
    if (rc != SQLITE_OK) {
        cerr << "SQL error: " << errMsg << endl;
        logger.error("SQL error: " + std::string(errMsg ? errMsg : "unknown"));
        sqlite3_free(errMsg);
        return false;
    }
    return true;
}

bool DatabaseManager::prepare(const std::string& sql, sqlite3_stmt** stmt) {
    int rc = sqlite3_open(dbName.c_str(), &db);
    if (rc != SQLITE_OK) {
        logger.error("Failed to open database: " + std::string(sqlite3_errmsg(db)));
    } else {
        logger.debug("Database opened successfully: " + std::string(dbName.c_str()));
    }
    logger.debug("Executing SQL: " + sql);
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, stmt, nullptr) != SQLITE_OK) {
        logger.error("SQL prepare failed: " + std::string(sqlite3_errmsg(db)));
        return false;
    }
    return true;
}


sqlite3* DatabaseManager::getDB() const { return db; }

DatabaseManager::~DatabaseManager() {
    close();
}