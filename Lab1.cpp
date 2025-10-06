#ifdef _WIN32
#include <windows.h>
void enableANSI() {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);
}
#endif
#include <string>
#include <iostream>
#include "Logger/Logger.h"
#include "Graphs/ZooGraph.h"
#include "Accounts/AuthManager.h"
#include "Menus/Menu.h"

using namespace std;

int main(int argc, char* argv[]) {
#ifdef _WIN32
    enableANSI();
#endif

    Logger logger("zoo_log.txt");
    logger.listEnabledLevels();

    logger.debug("System initialization");
    logger.info("Loading data from file complete");
    logger.warn("Insufficient memory for cache");
    logger.error("Failed to open database");

    //logger.disable(Logger::DEBUG); // Disable DEBUG messages
    //logger.disable(Logger::ERROR);
    //logger.enable(Logger::DEBUG);
    //logger.debug("This will not be printed");
    //logger.info("Continuing program execution");

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
