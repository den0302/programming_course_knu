#include <string>
#include <iostream>
#include "Employee.h"
#include "Logger.h"
#include "ZooGraph.h"

using namespace std;

int main(int argc, char* argv[]) {

    /*
    Logger logger;

    // Якщо користувач передав параметр
    if (argc > 1) {
        std::string arg = argv[1];
        logger.disable(Logger::DEBUG | Logger::INFO | Logger::WARN | Logger::ERROR); // вимикаємо все

        if (arg == "debug")   logger.enable(Logger::DEBUG);
        else if (arg == "info")  logger.enable(Logger::INFO);
        else if (arg == "warn")  logger.enable(Logger::WARN);
        else if (arg == "error") logger.enable(Logger::ERROR);
        else {
            cerr << "Невідомий рівень: " << arg << std::endl;
            return 1;
        }
    }

    logger.debug("Це повідомлення debug");
    logger.info("Це повідомлення info");
    logger.warn("Це повідомлення warn");
    logger.error("Це повідомлення error");*/

    Employee e1, e2, e3;
    e1.setId(1); e1.setAge(30); e1.setSalary(1000); e1.setExperience(5); e1.setPosition("Keeper"); e1.setName("Ivan");
    e2.setId(2); e2.setAge(25); e2.setSalary(900); e2.setExperience(2); e2.setPosition("Veterinarian"); e2.setName("Oksana");
    e3.setId(3); e3.setAge(40); e3.setSalary(1500); e3.setExperience(15); e3.setPosition("Guide"); e3.setName("Petro");

    ZooGraph zoo;

    auto a1 = make_shared<Aviary>("Savannah", "Lions", 300.0);
    auto a2 = make_shared<Aviary>("Jungle", "Monkeys", 150.0);
    auto a3 = make_shared<Aviary>("Lake", "Flamingos", 200.0);

    zoo.addAviary(a1);
    zoo.addAviary(a2);
    zoo.addAviary(a3);

    zoo.addPath(a1->getId(), a2->getId(), 50);
    zoo.addPath(a2->getId(), a3->getId(), 30);
    zoo.removePath(a2->getId(), a3->getId());

    zoo.printZoo();

    return 0;
}
