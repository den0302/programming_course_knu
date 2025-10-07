#ifndef EMPLOYEE_H
#define EMPLOYEE_H
#include <vector>
#include <string>
#include <memory>
#include <unordered_map>

using namespace std;

class ZooGraph;

class Employee {
private:
    string id;
    int age;
    int salary;
    int experience;
    string name;
    vector<string> aviaryIds;

public:
    Employee(const string& name, int age, int salary, int experience);

    string getId() const;
    int getAge() const;
    int getSalary() const;
    int getExperience() const;
    string getName() const;

    string getFullInfoAboutEmployee() const;
    string listAviaries() const ;
    void replaceAviary(const string& fromAviary, const string& toAviary);
    void removeAviary(const string& aviaryId);
    bool isAssigned() const;

    const vector<string>& getAviaryIds() const;
    void assignAviary(const string& aviary);
};

#endif //EMPLOYEE_H
