#ifndef ZOOGRAPH_H
#define ZOOGRAPH_H

#include <map>
#include <vector>
#include <string>
#include "Employee.h"

class ZooGraph {
private:
    map<string, vector<string>> adjList;                // список суміжності
    map<string, vector<Employee>> employeesInEnclosure; // працівники у вольєрах
public:
    void addEnclosure(string name);
    void removeEnclosure(string name);
    void addPath(string from, string to);
    void removePath(string from, string to);
    void assignEmployee(string enclosure, Employee e);
    void printGraph();
    void printEmployees();
};

#endif //ZOOGRAPH_H
