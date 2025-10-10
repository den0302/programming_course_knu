#include "PathRepository.h"
#include "../Graphs/ZooGraph.h"
#include "../Logger/LoggerGlobal.h"
using namespace std;

void PathRepository::initTable() {
    string sql = R"(
        CREATE TABLE IF NOT EXISTS Paths (
            fromId CHAR(36),
            toId CHAR(36),
            length DOUBLE PRECISION,
            PRIMARY KEY (fromId, toId),
            FOREIGN KEY (fromId) REFERENCES Aviaries(id) ON DELETE CASCADE,
            FOREIGN KEY (toId) REFERENCES Aviaries(id) ON DELETE CASCADE
        );
    )";
    db.execute(sql);
}

void PathRepository::addPath(const string& fromId, const string& toId, double length) {
    string sql =
        "INSERT OR REPLACE INTO Paths (fromId, toId, length) VALUES ('" +
        fromId + "', '" + toId + "', " + to_string(length) + ");";
    bool ok = db.execute(sql);
    if (ok) logger.info("Path added");
    else logger.error("Failed to insert Path");
}

void PathRepository::removePath(const string& fromId, const string& toId) {
    string sql =
        "DELETE FROM Paths WHERE fromId = '" + fromId + "' AND toId = '" + toId + "';";
    db.execute(sql);
}

vector<shared_ptr<Path>> PathRepository::getAllPaths() {
    vector<shared_ptr<Path>> paths;
    string sql = "SELECT fromId, toId, length FROM Paths;";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db.getDB(), sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        cerr << "SQL prepare error: " << sqlite3_errmsg(db.getDB()) << endl;
        return paths;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        string fromId   = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        string toId     = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        double length   = sqlite3_column_double(stmt, 2);

        auto path = make_shared<Path>(fromId, toId, length);
        paths.push_back(path);
    }

    sqlite3_finalize(stmt);
    return paths;
}

void PathRepository::clearAll() {
    db.execute("DELETE FROM Paths;");
}
