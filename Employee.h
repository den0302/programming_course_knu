
#ifndef EMPLOYEE_H
#define EMPLOYEE_H

#include <string>
using namespace std;

class Employee {
private:
    int id;
    int age;
    int salary;
    int experience;
    string position;
    string name;
public:
    int getId();
    int getAge();
    int getSalary();
    int getExperience();
    string getPosition();
    string getName();
    string getFullInfo();

    void setId(int id);
    void setAge(int age);
    void setSalary(int salary);
    void setExperience(int experience);
    void setPosition(string position);
    void setName(string name);
};


#endif //EMPLOYEE_H
