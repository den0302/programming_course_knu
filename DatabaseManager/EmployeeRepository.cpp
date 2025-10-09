#include "EmployeeRepository.h"
#include <memory>
#include <vector>
#include <sstream>

void EmployeeRepository::initTable() {
        string sql = R"(
            CREATE TABLE IF NOT EXISTS Employes (
                id CHAR(36) PRIMARY KEY,
                name TEXT,
                age INTEGER,
                salary INTEGER,
                experience INTEGER,
                assignedAviaries TEXT
            );
        )";
        db.execute(sql);

    string sql2 = R"(
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
    db.execute(sql2);
    }

    void EmployeeRepository::addEmployee(const Employee& e) {
    string sql = "INSERT INTO Employees (id, name, age, salary, experience, assignedAviaries) VALUES ("
             "'" + e.getId() + "', "
             "'" + e.getName() + "', "
             + to_string(e.getAge()) + ", "
             + to_string(e.getSalary()) + ", "
             + to_string(e.getExperience()) + ", "
             "'" + e.getAssignedAviaries() + "');";
    }

    void EmployeeRepository::removeEmployee(const string& id) {
        string sql = "DELETE FROM Employes WHERE id = '" + id + "';";
        db.execute(sql);
    }

    void EmployeeRepository::moveEmployee(const string& id, const string& newAviary) {
        string sql = "UPDATE Employes SET assignedAviaries = '" + newAviary + "' WHERE id = '" + id + "';";
        db.execute(sql);
    }

bool EmployeeRepository::assignEmployeeToAviary(const string& employeeId, const string& aviaryId) {
    sqlite3* dbConn = db.getDB();
    sqlite3_stmt* stmt;

    // --- 1. Призначити працівника у таблиці Aviaries ---
    const char* sqlAviary = "UPDATE Aviaries SET assignedEmployeeId = ? WHERE id = ?;";
    if (sqlite3_prepare_v2(dbConn, sqlAviary, -1, &stmt, nullptr) != SQLITE_OK) {
        // logger.error("SQL prepare error in assignEmployeeToAviary (Aviaries): " + string(sqlite3_errmsg(dbConn)));
        return false;
    }
    sqlite3_bind_text(stmt, 1, employeeId.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, aviaryId.c_str(), -1, SQLITE_STATIC);
    bool aviaryUpdated = (sqlite3_step(stmt) == SQLITE_DONE);
    sqlite3_finalize(stmt);

    if (!aviaryUpdated) {
        // logger.error("Failed to update Aviaries for employee " + employeeId);
        return false;
    }

    // --- 2. Оновити поле assignedAviaries у Employees ---
    const char* sqlSelect = "SELECT assignedAviaries FROM Employees WHERE id = ?;";
    if (sqlite3_prepare_v2(dbConn, sqlSelect, -1, &stmt, nullptr) != SQLITE_OK) {
        // logger.error("SQL prepare error in assignEmployeeToAviary (select): " + string(sqlite3_errmsg(dbConn)));
        return false;
    }
    sqlite3_bind_text(stmt, 1, employeeId.c_str(), -1, SQLITE_STATIC);

    string current;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        const unsigned char* text = sqlite3_column_text(stmt, 0);
        if (text) current = reinterpret_cast<const char*>(text);
    }
    sqlite3_finalize(stmt);

    if (!current.empty()) {
        if (current.find(aviaryId) == string::npos) current += "," + aviaryId;
    } else {
        current = aviaryId;
    }

    const char* sqlUpdate = "UPDATE Employees SET assignedAviaries = ? WHERE id = ?;";
    if (sqlite3_prepare_v2(dbConn, sqlUpdate, -1, &stmt, nullptr) != SQLITE_OK) {
        // logger.error("SQL prepare error in assignEmployeeToAviary (update Employees): " + string(sqlite3_errmsg(dbConn)));
        return false;
    }
    sqlite3_bind_text(stmt, 1, current.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, employeeId.c_str(), -1, SQLITE_STATIC);
    bool employeeUpdated = (sqlite3_step(stmt) == SQLITE_DONE);
    sqlite3_finalize(stmt);

    if (employeeUpdated) {
        // logger.info("Employee " + employeeId + " assigned to Aviary " + aviaryId);
        return true;
    }

    //logger.error("Failed to update Employees for employee " + employeeId);
    return false;
}

bool EmployeeRepository::removeEmployeeFromAviary(const string& employeeId, const string& aviaryId) {
    sqlite3* dbConn = db.getDB();
    sqlite3_stmt* stmt;

    // --- 1. Очистити поле assignedEmployeeId у Aviaries ---
    const char* sqlAviary = "UPDATE Aviaries SET assignedEmployeeId = NULL WHERE id = ?;";
    if (sqlite3_prepare_v2(dbConn, sqlAviary, -1, &stmt, nullptr) != SQLITE_OK) {
        //logger.error("SQL prepare error in removeEmployeeFromAviary (Aviaries): " + string(sqlite3_errmsg(dbConn)));
        return false;
    }
    sqlite3_bind_text(stmt, 1, aviaryId.c_str(), -1, SQLITE_STATIC);
    bool aviaryUpdated = (sqlite3_step(stmt) == SQLITE_DONE);
    sqlite3_finalize(stmt);

    if (!aviaryUpdated) {
        //logger.error("Failed to update Aviaries for aviary " + aviaryId);
        return false;
    }

    // --- 2. Видалити aviaryId з assignedAviaries у Employees ---
    const char* sqlSelect = "SELECT assignedAviaries FROM Employees WHERE id = ?;";
    if (sqlite3_prepare_v2(dbConn, sqlSelect, -1, &stmt, nullptr) != SQLITE_OK) {
        // logger.error("SQL prepare error in removeEmployeeFromAviary (select): " + string(sqlite3_errmsg(dbConn)));
        return false;
    }
    sqlite3_bind_text(stmt, 1, employeeId.c_str(), -1, SQLITE_STATIC);

    string current;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        const unsigned char* text = sqlite3_column_text(stmt, 0);
        if (text) current = reinterpret_cast<const char*>(text);
    }
    sqlite3_finalize(stmt);

    if (!current.empty()) {
        string updated;
        stringstream ss(current);
        string token;
        bool first = true;
        while (getline(ss, token, ',')) {
            if (token != aviaryId) {
                if (!first) updated += ",";
                updated += token;
                first = false;
            }
        }

        const char* sqlUpdate = "UPDATE Employees SET assignedAviaries = ? WHERE id = ?;";
        if (sqlite3_prepare_v2(dbConn, sqlUpdate, -1, &stmt, nullptr) != SQLITE_OK) {
            //logger.error("SQL prepare error in removeEmployeeFromAviary (update Employees): " + string(sqlite3_errmsg(dbConn)));
            return false;
        }
        sqlite3_bind_text(stmt, 1, updated.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, employeeId.c_str(), -1, SQLITE_STATIC);
        bool employeeUpdated = (sqlite3_step(stmt) == SQLITE_DONE);
        sqlite3_finalize(stmt);

        if (employeeUpdated) {
            //logger.info("Removed Aviary " + aviaryId + " from Employee " + employeeId);
            return true;
        }
    }

    //logger.warn("No aviary found to remove for employee " + employeeId);
    return true;
}

    unordered_map<string, shared_ptr<Employee>> EmployeeRepository::getAllEmployes() {
        unordered_map<string, shared_ptr<Employee>> employees;
        string sql = "SELECT id, age, salary, experience, name, assignedAviaries FROM Employes;";
        sqlite3_stmt* stmt;

        if (sqlite3_prepare_v2(db.getDB(), sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
            cerr << "Request preparation error: " << sqlite3_errmsg(db.getDB()) << endl;
            return employees;
        }

        while (sqlite3_step(stmt) == SQLITE_ROW) {
            string id =           reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
            string name =         reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
            int age =             sqlite3_column_int(stmt, 2);
            int salary =          sqlite3_column_int(stmt, 3);
            int experience =      sqlite3_column_int(stmt, 4);
            string assigned =     sqlite3_column_text(stmt, 5) ? reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5)) : "";

            auto emp = make_shared<Employee>(id, name, age, salary, experience, assigned);
            employees[id] = emp;
        }

        sqlite3_finalize(stmt);
        return employees;
    }

void EmployeeRepository::clearAll() {
    db.execute("DELETE FROM Employes;");
}