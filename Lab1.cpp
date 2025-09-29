#include <iostream>
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
    int getId() {
        return id;
    }
    int getAge() {
        return age;
    }
    int getSalary() {
        return salary;
    }
    int getExperience() {
        return experience;
    }
    string getPosition() {
        return position;
    }
    string getName() {
        return name;
    }
    string getFullInfo() {
        return "ID: " + to_string(id) + " Age: " + to_string(age) + " Salary: " + to_string(salary) + " Experience: " + to_string(experience) + " Position: " + position + " Name: " + name + "\n";
    }
    void setId(int id) {
        this->id = id;
    }
    void setAge(int age) {
        if (age<0) {
            cout << "Age can not be less than 0" << endl;
        }else {
            this->age = age;
        }
    }
    void setSalary(int salary) {
        this->salary = salary;
    }
    void setExperience(int experience) {
        this->experience = experience;
    }
    void setPosition(string position) {
        this->position = position;
    }
    void setName(string name) {
        if (!name.empty())
            this->name = name;
    }
};

int main() {
    int id;
    int age;
    int salary;
    int experience;
    string position;
    string name;

    cout << "Enter employee: " << endl;
    cin>>id>>age>>salary>>experience;
    position = cin.get();
    name = cin.get();
    Employee employee;
    employee.setId(id);
    employee.setAge(age);
    employee.setSalary(salary);
    employee.setExperience(experience);
    employee.setPosition(position);
    employee.setName(name);
    cout << employee.getFullInfo() << endl;

    return 0;
}