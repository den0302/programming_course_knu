#include "AviaryRepository.h"
#include "../Graphs/ZooGraph.h"
#include "../Logger/LoggerGlobal.h"
#include "SQLUtilities/SQLUtils.h"
using namespace std;

void AviaryRepository::initTable() {
    string sql = R"(
        CREATE TABLE IF NOT EXISTS Aviaries (
            id CHAR(36) PRIMARY KEY,
            name TEXT,
            type TEXT,
            area DOUBLE PRECISION,
            capacity INTEGER,
            animals TEXT,
            assignedEmployeeId CHAR(36),
            FOREIGN KEY (assignedEmployeeId) REFERENCES Employes(id) ON DELETE SET NULL
        );
    )";
    db.execute(sql);
    if (!db.getDB()) {
        logger.error("Database pointer is null before initTable!");
        return;
    }
    const char* dbFile = sqlite3_db_filename(db.getDB(), "main");
    logger.debug(string("AviaryRepository::initTable - DB file: ") + (dbFile ? dbFile : "unknown"));

    if (!db.execute(sql)) {
        logger.error("Failed to create table 'Aviaries'. See previous SQL error.");
        return;
    }
    logger.info("Table 'Aviaries' ensured.");
}

void AviaryRepository::addAviary(const Aviary& a) {
    string safeName = escapeSQL(a.getName());
    string safeType = escapeSQL(a.getType());

    string sql =
        "INSERT INTO Aviaries (id, name, type, area, capacity, animals, assignedEmployeeId) VALUES ('" +
            a.getIdAviary() + "', '" +
            safeName + "', '" +
            safeType + "', " +
            to_string(a.getArea()) + ", " +
            to_string(a.getCapacity()) + ", '" +
            a.getAnimalsStr() + "', " +
            (!a.getAssignedEmployee().empty() ? ("'" + a.getAssignedEmployee() + "'") : "NULL") +
            ");";
    bool ok = db.execute(sql);
    if (ok) logger.info("Aviary added: " + a.getIdAviary());
    else logger.error("Failed to insert Aviary: " + a.getIdAviary());\
}

void AviaryRepository::removeAviary(const string& id) {
    string sql = "DELETE FROM Aviaries WHERE id = '" + id + "';";
    db.execute(sql);
}

void AviaryRepository::updateAviaryEmployee(const string& aviaryId, const string& employeeId) {
    string sql =
        "UPDATE Aviaries SET assignedEmployeeId = '" + employeeId +
        "' WHERE id = '" + aviaryId + "';";
    db.execute(sql);
}

unordered_map<string, shared_ptr<Aviary>> AviaryRepository::getAllAviaries() {
    unordered_map<string, shared_ptr<Aviary>> aviaries;
    string sql = "SELECT id, name, type, area, capacity, animals, assignedEmployeeId FROM Aviaries;";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db.getDB(), sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        cerr << "SQL prepare error: " << sqlite3_errmsg(db.getDB()) << endl;
        return aviaries;
    }

    auto getTextSafe = [&](int col) -> string {
        const unsigned char* txt = sqlite3_column_text(stmt, col);
        return txt ? reinterpret_cast<const char*>(txt) : "";
    };

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        string id          = getTextSafe(0);
        string name        = getTextSafe(1);
        string type        = getTextSafe(2);
        double area        = sqlite3_column_double(stmt, 3);
        int capacity       = sqlite3_column_int(stmt, 4);
        string animals     = getTextSafe(5);
        string assignedId  = getTextSafe(6);

        auto aviary = make_shared<Aviary>(id, name, type, area, capacity, assignedId, animals);
        aviaries[id] = aviary;

        logger.info("Loaded Aviary from DB with id: " + id +
                    " (animals pending load: " + animals + ")");
    }

    sqlite3_finalize(stmt);
    return aviaries;
}


void AviaryRepository::clearAll() {
    db.execute("DELETE FROM Aviaries;");
}
