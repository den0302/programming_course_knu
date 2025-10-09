#ifndef EMPLOYEE_H
#define EMPLOYEE_H
#include <vector>
#include <string>

using namespace std;

class ZooGraph;

class Employee {
private:
    string id;
    string name;
    int age;
    int salary;
    int experience;
    vector<string> aviaryIds;

public:
    Employee(const string& id, const string& name, int age, int salary, int experience, const string& aviaryIds);
    Employee(const string& name, int age, int salary, int experience);

    [[nodiscard]] string getId() const;
    [[nodiscard]] int getAge() const;
    [[nodiscard]] int getSalary() const;
    [[nodiscard]] int getExperience() const;
    [[nodiscard]] string getName() const;

    [[nodiscard]] string getAssignedAviaries() const;
    void setAssignedAviaries(const string& assignedAviaries);
    void setAssignedAviaries(vector<string> assignedAviaries);
    vector<string> assignedAviaries(const string& assignedAviaries);


    [[nodiscard]] string getFullInfoAboutEmployee() const;
    [[nodiscard]] string listAviaries() const ;
    void replaceAviary(const string& fromAviary, const string& toAviary);
    void removeAviary(const string& aviaryId);
    [[nodiscard]] bool isAssigned() const;

    [[nodiscard]] const vector<string>& getAviaryIds() const;
    void assignAviary(const string& aviary);
};

#endif //EMPLOYEE_H
