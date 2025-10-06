#include <string>
#include <iostream>
#include "Logger.h"
#include "Graphs/ZooGraph.h"
#include "Accounts/AuthManager.h"
#include "Menus/Menu.h"

using namespace std;

int main(int argc, char* argv[]) {


    /*
    Logger logger;

    // Якщо користувач передав параметр
    if (argc > 1) {
        string arg = argv[1];
        logger.disable(Logger::DEBUG | Logger::INFO | Logger::WARN | Logger::ERROR); // вимикаємо все

        if (arg == "debug")   logger.enable(Logger::DEBUG);
        else if (arg == "info")  logger.enable(Logger::INFO);
        else if (arg == "warn")  logger.enable(Logger::WARN);
        else if (arg == "error") logger.enable(Logger::ERROR);
        else {
            cerr << "Невідомий рівень: " << arg << endl;
            return 1;
        }
    }

    logger.debug("Це повідомлення debug");
    logger.info("Це повідомлення info");
    logger.warn("Це повідомлення warn");
    logger.error("Це повідомлення error");*/

    AuthManager auth;
    ZooGraph zoo;

    auto emp1 = make_shared<Employee>("John Smith", 34, 1200, 7);
    auto emp2 = make_shared<Employee>("Alice Brown", 29, 950, 4);
    zoo.getEmployeeManager().addEmployee(emp1);
    zoo.getEmployeeManager().addEmployee(emp2);

    auto av1 = make_shared<Aviary>("Savanna Zone", "Mammal", 120.5, 5);
    auto av2 = make_shared<Aviary>("Bird Paradise", "Bird", 60.0, 10);
    auto lionAviary = make_shared<Aviary>("Lion's Den", "Mammal", 100.0, 5);
    auto birdHouse = make_shared<Aviary>("Bird House", "Bird", 50.0, 10);
    auto aquarium = make_shared<Aviary>("Aquarium", "Fish", 200.0, 20);

    zoo.addAviary(av1);
    zoo.addAviary(av2);
    zoo.addAviary(lionAviary);
    zoo.addAviary(birdHouse);
    zoo.addAviary(aquarium);

    zoo.getEmployeeManager().assignEmployeeToAviary(emp1->getId(), av1->getId());
    zoo.getEmployeeManager().assignEmployeeToAviary(emp2->getId(), av2->getId());



    // types: Mammal Reptile Bird Fish Amphibian Arachnid Insect
    zoo.getAnimalManager().createAnimal("Leo", "Lion", 5, 190.0, "Mammal");
    zoo.getAnimalManager().createAnimal("Polly", "Parrot", 2, 1.0, "Bird");
    zoo.getAnimalManager().createAnimal("Nemo", "Clownfish", 1, 0.2, "Fish");
    zoo.getAnimalManager().createAnimal("Frog", "Frog", 1, 0.5, "Amphibian");
    zoo.getAnimalManager().createAnimal("Spidey", "Spider", 1, 0.1, "Arachnid");
    zoo.getAnimalManager().createAnimal("Buzz", "Bee", 0, 0.05, "Insect");

    auto animals = zoo.getAnimalManager().getAnimalsNotInAviaries();
    for (auto& a : animals) {
        if (a->getSpecies() == "Lion") zoo.getAnimalManager().addAnimalInAviary(av1->getId(), a->getId());
        else if (a->getSpecies() == "Parrot") zoo.getAnimalManager().addAnimalInAviary(birdHouse->getId(), a->getId());
        else if (a->getSpecies() == "Clownfish") zoo.getAnimalManager().addAnimalInAviary(aquarium->getId(), a->getId());
    }

    zoo.addPath(lionAviary->getId(), birdHouse->getId(), 50);
    zoo.addPath(birdHouse->getId(), aquarium->getId(), 30);

    zoo.listEmployees();
    zoo.listAnimals();

    zoo.printZoo();

    while (true) {
        auto res = auth.authenticateInteractive();
        if (!res.has_value()) {
            cout << "Try again? (y/n): ";
            char c; cin >> c;
            if (c == 'y' || c == 'Y') continue;
            break; 
        }

        string username = res->first;
        Role role = res->second;

        Menu::showFor(username, role, auth, zoo );

        cout << "Exit the program or return to login? (q - exit, any other key - login): ";
        char c; cin >> c;
        if (c == 'q' || c == 'Q') break;
    }

    cout << "Bye!\n";

    
    return 0;
}
