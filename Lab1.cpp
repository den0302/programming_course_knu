#include <string>
#include <iostream>
#include "Employee.h"
#include "Logger.h"
#include "ZooGraph.h"
#include "Creatures.h"

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

    /*Employee e1, e2, e3;
    e1.setId(1); e1.setAge(30); e1.setSalary(1000); e1.setExperience(5); e1.setPosition("Keeper"); e1.setName("Ivan");
    e2.setId(2); e2.setAge(25); e2.setSalary(900); e2.setExperience(2); e2.setPosition("Veterinarian"); e2.setName("Oksana");
    e3.setId(3); e3.setAge(40); e3.setSalary(1500); e3.setExperience(15); e3.setPosition("Guide"); e3.setName("Petro");*/

    auto zooGraph = make_shared<ZooGraph>();
    AnimalManager manager(*zooGraph);

    auto lionAviary = make_shared<Aviary>("Lion's Den", "Mammal", 100.0, 5);
    auto birdHouse = make_shared<Aviary>("Bird House", "Bird", 50.0, 10);
    auto aquarium = make_shared<Aviary>("Aquarium", "Fish", 200.0, 20);

    zooGraph->addAviary(lionAviary);
    zooGraph->addAviary(birdHouse);
    zooGraph->addAviary(aquarium);


    // types: Mammal Reptile Bird Fish Amphibian Arachnid Insect

    manager.createAnimal("Leo", "Lion", 5, 190.0, "Mammal");
    manager.createAnimal("Polly", "Parrot", 2, 1.0, "Bird");
    manager.createAnimal("Nemo", "Clownfish", 1, 0.2, "Fish");
    manager.createAnimal("Frog", "Frog", 1, 0.5, "Amphibian");
    manager.createAnimal("Spidey", "Spider", 1, 0.1, "Arachnid");
    manager.createAnimal("Buzz", "Bee", 0, 0.05, "Insect");

    auto animals = manager.getAnimalsNotInAviaries();
    for (auto& a : animals) {
        if (a->getSpecies() == "Lion") manager.addAnimalInAviary(lionAviary->getId(), a->getId());
        else if (a->getSpecies() == "Parrot") manager.addAnimalInAviary(birdHouse->getId(), a->getId());
        else if (a->getSpecies() == "Clownfish") manager.addAnimalInAviary(aquarium->getId(), a->getId());
    }

    manager.listAllAnimals();

    lionAviary->listAnimals();
    birdHouse->listAnimals();
    aquarium->listAnimals();

    zooGraph->addPath(lionAviary->getId(), birdHouse->getId(), 50);
    zooGraph->addPath(birdHouse->getId(), aquarium->getId(), 30);
    zooGraph->printZoo();

    return 0;
}
