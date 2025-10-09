#include "AviaryRepository.h"

void AviaryRepository::initTable() {
    std::string sql = R"(
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
}

void AviaryRepository::addAviary(const Aviary& a) {
    std::string sql =
        "INSERT INTO Aviaries (id, name, type, area, capacity, animals, assignedEmployeeId) VALUES ('" +
            a.getIdAviary() + "', '" +
            a.getName() + "', '" +
            a.getType() + "', " +
            std::to_string(a.getArea()) + ", " +
            std::to_string(a.getCapacity()) + ", '" +
            a.getAnimalsStr() + "', " +
            (!a.getAssignedEmployee().empty() ? ("'" + a.getAssignedEmployee() + "'") : "NULL") +
            ");";
    db.execute(sql);
}

void AviaryRepository::removeAviary(const std::string& id) {
    std::string sql = "DELETE FROM Aviaries WHERE id = '" + id + "';";
    db.execute(sql);
}

void AviaryRepository::updateAviaryEmployee(const std::string& aviaryId, const std::string& employeeId) {
    std::string sql =
        "UPDATE Aviaries SET assignedEmployeeId = '" + employeeId +
        "' WHERE id = '" + aviaryId + "';";
    db.execute(sql);
}

std::unordered_map<std::string, std::shared_ptr<Aviary>> AviaryRepository::getAllAviaries() {
    std::unordered_map<std::string, std::shared_ptr<Aviary>> aviaries;
    std::string sql = "SELECT id, name, type, area, capacity, assignedEmployeeId FROM Aviaries;";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db.getDB(), sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "SQL prepare error: " << sqlite3_errmsg(db.getDB()) << std::endl;
        return aviaries;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        string id =         reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        string name =       reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        string type =       reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        double area =       sqlite3_column_double(stmt, 3);
        int capacity =      sqlite3_column_int(stmt, 4);
        string animals =    sqlite3_column_text(stmt, 5) ? reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5)) : "";

        string assignedId;
        if (sqlite3_column_text(stmt, 5)) assignedId = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 6));

        auto aviary = make_shared<Aviary>(id, name, type, area, capacity, assignedId, animals);
        aviaries[id] = aviary;
    }

    sqlite3_finalize(stmt);
    return aviaries;
}

void AviaryRepository::clearAll() {
    db.execute("DELETE FROM Aviaries;");
}
