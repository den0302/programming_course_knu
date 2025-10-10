#ifndef EMPLOYEEREPOSITORY_H
#define EMPLOYEEREPOSITORY_H
#include "DatabaseManager.h"
#include <unordered_map>
#include <memory>

using namespace std;

class EmployeeManager;
class Employee;

class EmployeeRepository {
private:
    DatabaseManager& db;

public:
    EmployeeRepository(DatabaseManager& database) : db(database) {}

    void initTable() ;

    void addEmployee(const Employee& e);
    void removeEmployee(const string& id);
    void moveEmployee(const string& employeeId, const string& oldAviary, const string& newAviary);
    bool assignEmployeeToAviary(const string& employeeId, const string& aviaryId);
    bool removeEmployeeFromAviary(const string& employeeId, const string& aviaryId);

    unordered_map<string, shared_ptr<Employee>> getAllEmployes();

    void clearAll();
};

#endif //EMPLOYEEREPOSITORY_H
