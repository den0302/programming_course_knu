#include <iostream>
#include <map>
#include <vector>
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
        return "ID: " + to_string(id) + " Age: " + to_string(age) +
               " Salary: " + to_string(salary) + " Experience: " +
               to_string(experience) + " Position: " + position +
               " Name: " + name + "\n";
    }
    void setId(int id) {
        this->id = id;
    }
    void setAge(int age) {
        if (age < 0) {
            cout << "Age can not be less than 0" << endl;
        } else {
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

class ZooGraph {
private:
    map<string, vector<string>> adjList; // список суміжності
    map<string, vector<Employee>> employeesInEnclosure;
public:

    void addEnclosure(string name) {
        if (adjList.find(name) == adjList.end()) {
            adjList[name] = {};
            cout << "Added enclosure: " << name << endl;
        }
    }

    void removeEnclosure(string name) {
        if (adjList.find(name) != adjList.end()) {
            adjList.erase(name);
            employeesInEnclosure.erase(name);
            for (auto &p : adjList) {
                auto &vec = p.second;
                vec.erase(remove(vec.begin(), vec.end(), name), vec.end());
            }
            cout << "Removed enclosure: " << name << endl;
        }
    }

    void addPath(string from, string to) {
        if (adjList.find(from) != adjList.end() && adjList.find(to) != adjList.end()) {
            adjList[from].push_back(to);
            adjList[to].push_back(from);
            cout << "Added path between " << from << " and " << to << endl;
        }
    }

    void removePath(string from, string to) {
        if (adjList.find(from) != adjList.end() && adjList.find(to) != adjList.end()) {
            auto &vec1 = adjList[from];
            auto &vec2 = adjList[to];
            vec1.erase(remove(vec1.begin(), vec1.end(), to), vec1.end());
            vec2.erase(remove(vec2.begin(), vec2.end(), from), vec2.end());
            cout << "Removed path between " << from << " and " << to << endl;
        }
    }

    void assignEmployee(string enclosure, Employee e) {
        if (adjList.find(enclosure) != adjList.end()) {
            employeesInEnclosure[enclosure].push_back(e);
            cout << "Assigned " << e.getName() << " to " << enclosure << endl;
        }
    }

    void printEmployees() {
        cout << "\nEmployees in enclosures:" << endl;
        for (auto &p : employeesInEnclosure) {
            cout << p.first << ": " << endl;
            for (auto &emp : p.second) {
                cout << "   " << emp.getFullInfo();
            }
        }
    }
};

int main() {
    Employee e1, e2, e3;
    e1.setId(1); e1.setAge(30); e1.setSalary(1000); e1.setExperience(5); e1.setPosition("Keeper"); e1.setName("Ivan");
    e2.setId(2); e2.setAge(25); e2.setSalary(900); e2.setExperience(2); e2.setPosition("Veterinarian"); e2.setName("Oksana");
    e3.setId(3); e3.setAge(40); e3.setSalary(1500); e3.setExperience(15); e3.setPosition("Guide"); e3.setName("Petro");

    // граф
    ZooGraph zoo;
    zoo.addEnclosure("Lions");
    zoo.addEnclosure("Elephants");
    zoo.addEnclosure("Birds");

    zoo.addPath("Lions", "Elephants");
    zoo.addPath("Elephants", "Birds");

    zoo.assignEmployee("Lions", e1);
    zoo.assignEmployee("Elephants", e2);
    zoo.assignEmployee("Birds", e3);

    zoo.printEmployees();

    return 0;
}
