#ifndef EMPLOYEEMANAGER_H
#define EMPLOYEEMANAGER_H
#include "Employee.h"

class EmployeeManager {
private:
    ZooGraph& zooGraph;
    unordered_map<string, shared_ptr<Employee>> employees;
public:
    explicit EmployeeManager(ZooGraph& graph) : zooGraph(graph) {}

    bool addEmployee(const shared_ptr<Employee>& employee);

    shared_ptr<Employee> getEmployee(const string& id) const;
    const unordered_map<string, shared_ptr<Employee>>& getAllEmployees() const;

    bool assignEmployeeToAviary(const string& employeeId, const string& aviaryId);
    bool removeEmployee(const string& id);

    void listAllEmployees() const;
};
#endif //EMPLOYEEMANAGER_H
