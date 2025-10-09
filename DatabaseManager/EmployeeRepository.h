#ifndef EMPLOYEEREPOSITORY_H
#define EMPLOYEEREPOSITORY_H
#include "DatabaseManager.h"
#include "../Creatures/EmployeeManager.h"
#include <unordered_map>


class EmployeeRepository {
private:
    DatabaseManager& db;

public:
    EmployeeRepository(DatabaseManager& database) : db(database) {}

    void initTable() ;

    void addEmployee(const Employee& e);
    void removeEmployee(const string& id);
    void moveEmployee(const string& id, const string& newAviary);
    bool assignEmployeeToAviary(const std::string& employeeId, const std::string& aviaryId);
    bool removeEmployeeFromAviary(const std::string& employeeId, const std::string& aviaryId);

    unordered_map<string, shared_ptr<Employee>> getAllEmployes();

    void clearAll();
};

#endif //EMPLOYEEREPOSITORY_H
