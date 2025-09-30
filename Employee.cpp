#include "Employee.h"
#include <iostream>

int Employee::getId() {
    return id;
}
int Employee::getAge() {
    return age;
}
int Employee::getSalary() {
    return salary;
}
int Employee::getExperience() {
    return experience;
}
string Employee::getPosition() {
    return position;
}
string Employee::getName() {
    return name;
}
string Employee::getFullInfo() {
    return "ID: " + to_string(id) + " Age: " + to_string(age) +
           " Salary: " + to_string(salary) + " Experience: " +
           to_string(experience) + " Position: " + position +
           " Name: " + name + "\n";
}
void Employee::setId(int id) {
    this->id = id;
}
void Employee::setAge(int age) {
    if (age < 0) {
        cout << "Age can not be less than 0" << endl;
    } else {
        this->age = age;
    }
}
void Employee::setSalary(int salary) {
    this->salary = salary;
}
void Employee::setExperience(int experience) {
    this->experience = experience;
}
void Employee::setPosition(string position) {
    this->position = position;
}
void Employee::setName(string name) {
    if (!name.empty())
        this->name = name;
}
