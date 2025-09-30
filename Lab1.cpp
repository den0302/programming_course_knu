#include <iostream>
#include <map>
#include <vector>
#include <algorithm>
#include <string>
#include "Employee.h"
#include "ZooGraph.h"

using namespace std;

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
