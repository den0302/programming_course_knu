#ifndef EMPLOYEEMANAGER_H
#define EMPLOYEEMANAGER_H
#include "Employee.h"
#include "../DatabaseManager/EmployeeRepository.h"
#include <unordered_map>
#include <memory>

class EmployeeManager {
private:
    ZooGraph& zooGraph;
    EmployeeRepository& employeeRepo;
    unordered_map<string, shared_ptr<Employee>> employees;
public:
    EmployeeManager(ZooGraph& graph, EmployeeRepository& employeeRepo);

    void loadEmployeesFromRepo(EmployeeRepository& employeeRepo);

    shared_ptr<Employee> getEmployee(const string& id) const;
    const unordered_map<string, shared_ptr<Employee>>& getAllEmployees() const;
    vector<shared_ptr<Employee>> getUnassignedEmployees() const;

    bool addEmployee(const shared_ptr<Employee>& employee);
    bool assignEmployeeToAviary(const string& employeeId, const string& aviaryId);//updateAviaryEmployee in Aviary?ad
    bool removeEmployee(const string& id);
    bool reassignEmployee(const string& empId, const string& fromAviaryId, const string& toAviaryId); //updateAviaryEmployee in Aviary
    bool removeEmployeeFromAviary(const string& employeeId, const string& aviaryId);//updateAviaryEmployee in Aviary

    void listAllEmployees() const;
};
#endif //EMPLOYEEMANAGER_H
