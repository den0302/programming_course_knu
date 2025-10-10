#include <iostream>
#include <string>
#include <sstream>
#include <limits>
#include "AdminMenu.h"
#include "../Logger/LoggerGlobal.h"
using namespace std;

void AdminMenu::show(const string& username, AuthManager& auth, ZooGraph& zoo) {
    bool running = true;
    while (running) {
        cout << "\n--- Admin Menu (logged as " << username << ") ---\n"
                "1. Create account\n"
                "2. Delete account\n"
                "3. Edit account\n"
                "4. Create animal\n"
                "5. Delete animal\n"
                "6. Create employee\n"
                "7. Delete employee\n"
                "8. Create aviary\n"
                "9. Delete aviary\n"
                "10. Add Path\n"
                "11. Remove path\n"
                "12. Is Zoo connected\n"
                "13. Print path between aviaries\n"
                "14. List all animals\n"
                "15. List all employees\n"
                "16. List all aviaries\n"
                "17. Print Zoo\n"
                "18. Log settings\n"
                "19. Return to login screen\n"
                "========================================================\n"
                "Choice: ";

        int ch; cin >> ch;

        switch (ch) {
            case 1: {
                string u, p; int r;
                cout << "New login: "; cin >> u;
                cout << "Password: "; cin >> p;
                cout << "Role (0=Admin,1=Manager,2=Employee): "; cin >> r;
                Role role = Account::intToRole(r);

                ostringstream oss;
                if (auth.addAccount(u, p, role)) {
                    oss << "[" << username << "] Created account: " << u
                        << " (Role: " << Account::roleToString(role) << ")";
                    logger.info(oss.str());
                } else {
                    oss << "[" << username << "] Failed to create account (already exists): " << u;
                    logger.warn(oss.str());
                }
                break;
            }

            case 2: {
                string u;
                cout << "Login to delete: "; cin >> u;
                ostringstream oss;
                if (auth.removeAccount(u)) {
                    oss << "[" << username << "] Deleted account: " << u;
                    logger.info(oss.str());
                } else {
                    oss << "[" << username << "] Tried to delete non-existing account: " << u;
                    logger.warn(oss.str());
                }
                break;
            }

            case 3: {
                string u;
                cout << "Enter account login to edit: ";
                cin >> u;
                bool result = auth.editAccount(u);
                ostringstream oss;
                if (result) {
                    oss << "[" << username << "] Edited account: " << u;
                    logger.info(oss.str());
                } else {
                    oss << "[" << username << "] Failed to edit account: " << u;
                    logger.warn(oss.str());
                }
                break;
            }

            case 4: {
                int age;
                double weight;
                string name, species, type;
                cout << "Name: ";
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                getline(cin, name);
                cout << "Species: ";
                getline(cin, species);
                cout << "Age: "; cin >> age;
                cout << "Weight: "; cin >> weight;
                cout << "Type(Mammal Reptile Bird Fish Amphibian Arachnid Insect): "; cin >> type;
                if (type != "Mammal" &&
                    type !=  "Reptile" &&
                    type !=   "Bird" &&
                    type !=   "Fish" &&
                    type !=   "Amphibian" &&
                    type !=   "Arachnid" &&
                    type !=   "Insect") {
                    logger.error("Wrong type animal"); break;
                }
                zoo.getAnimalManager().createAnimal(name, species, age, weight, type);
                ostringstream oss;
                oss << "[" << username << "] Created animal: " << name << " (" << species << ")";
                logger.info(oss.str());
                break;
            }

            case 5: {
                string id;
                cout << "Animal ID to delete: "; cin >> id;
                zoo.getAnimalManager().removeAnimalFromAnimals(id);
                ostringstream oss;
                oss << "[" << username << "] Removed animal with ID: " << id;
                logger.info(oss.str());
                break;
            }

            case 6: {
                string name;
                int salary, age, experience;
                cout << "Name: ";
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                getline(cin, name);
                cout << "Salary: "; cin >> salary;
                cout << "Age: "; cin >> age;
                cout << "Experience: "; cin >> experience;
                zoo.getEmployeeManager().addEmployee(make_shared<Employee>(name, age, salary, experience));
                ostringstream oss;
                oss << "[" << username << "] Added employee: " << name;
                logger.info(oss.str());
                break;
            }

            case 7: {
                string id;
                cout << "Employee ID to delete: "; cin >> id;
                zoo.getEmployeeManager().removeEmployee(id);
                ostringstream oss;
                oss << "[" << username << "] Deleted employee with ID: " << id;
                logger.info(oss.str());
                break;
            }

            case 8: {
                string name, type;
                double area;
                int capacity;
                cout << "Name: ";
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                getline(cin, name);
                cout << "Type: ";
                getline(cin, type);
                cout << "Area: "; cin >> area;
                cout << "Capacity: "; cin >> capacity;
                zoo.addAviary(make_shared<Aviary>(name, type, area, capacity));
                ostringstream oss;
                oss << "[" << username << "] Created aviary: " << name << " (Type: " << type << ")";
                logger.info(oss.str());
                break;
            }

            case 9: {
                string id;
                cout << "Aviary ID to delete: "; cin >> id;
                zoo.removeAviary(id);
                ostringstream oss;
                oss << "[" << username << "] Deleted aviary with ID: " << id;
                logger.info(oss.str());
                break;
            }

            case 10: {
                int length;
                string fromId, toId;
                cout << "From aviary ID: "; cin >> fromId;
                cout << "To aviary ID: "; cin >> toId;
                cout << "Length: "; cin >> length;
                zoo.addPath(fromId, toId, length);
                ostringstream oss;
                oss << "[" << username << "] Added path: " << fromId << " ↔ " << toId
                    << " (" << length << " m)";
                logger.info(oss.str());
                break;
            }

            case 11: {
                string fromId, toId;
                cout << "From aviary ID: "; cin >> fromId;
                cout << "To aviary ID: "; cin >> toId;
                zoo.removePath(fromId, toId);
                ostringstream oss;
                oss << "[" << username << "] Removed path: " << fromId << " ↔ " << toId;
                logger.info(oss.str());
                break;
            }

            case 12: {
                bool connected = zoo.isZooConnected();
                cout << (connected ? "Connected" : "Disconnected") << endl;
                ostringstream oss;
                oss << "[" << username << "] Checked zoo connectivity: "
                    << (connected ? "Connected" : "Disconnected");
                logger.debug(oss.str());
                break;
            }

            case 13: {
                string fromId, toId;
                cout << "From aviary ID: "; cin >> fromId;
                cout << "To aviary ID: "; cin >> toId;
                zoo.printPathBetweenAviaries(fromId, toId);
                ostringstream oss;
                oss << "[" << username << "] Printed path between: " << fromId << " and " << toId;
                logger.debug(oss.str());
                break;
            }

            case 14:
                zoo.listAnimals();
                logger.debug("[" + username + "] Viewed animal list.");
                break;

            case 15:
                zoo.listEmployees();
                logger.debug("[" + username + "] Viewed employee list.");
                break;

            case 16:
                zoo.printAviaries();
                logger.debug("[" + username + "] Viewed aviary list.");
                break;

            case 17:
                zoo.printZoo();
                logger.debug("[" + username + "] Viewed full zoo map.");
                break;

            case 18: {
                logger.listEnabledLevels();
                cout << "1. Enable Level\n2. Disable Level\nAny button to move back \nChoice: ";
                int ch2; cin >> ch2;

                switch (ch2) {
                    case 1: {
                        cout << "1. Enable DEBUG\n2. Enable INFO\n3. Enable WARN\n4. Enable ERROR\nChoice: ";
                        int ch3; cin >> ch3;
                        switch (ch3) {
                            case 1: logger.enable(Logger::DEBUG); break;
                            case 2: logger.enable(Logger::INFO); break;
                            case 3: logger.enable(Logger::WARN); break;
                            case 4: logger.enable(Logger::ERROR); break;
                            default: cout << "Wrong choice\n";
                        }
                        ostringstream oss;
                        oss << "[" << username << "] Enabled logger level option.";
                        logger.info(oss.str());
                        break;
                    }
                    case 2: {
                        cout << "1. Disable DEBUG\n2. Disable INFO\n3. Disable WARN\n4. Disable ERROR\nChoice: ";
                        int ch3; cin >> ch3;
                        switch (ch3) {
                            case 1: logger.disable(Logger::DEBUG); break;
                            case 2: logger.disable(Logger::INFO); break;
                            case 3: logger.disable(Logger::WARN); break;
                            case 4: logger.disable(Logger::ERROR); break;
                            default: cout << "Wrong choice\n";
                        }
                        ostringstream oss;
                        oss << "[" << username << "] Disabled logger level option.";
                        logger.info(oss.str());
                        break;
                    }
                    default:
                        cout << "Wrong choice\n";
                        ostringstream oss;
                        oss << "[" << username << "] Entered invalid log settings option.";
                        logger.warn(oss.str());
                }
                break;
            }

            case 19: {
                ostringstream oss;
                oss << "[" << username << "] Logged out and returned to login screen.";
                logger.info(oss.str());
                running = false;
                break;
            }

            default: {
                cout << "Wrong choice\n";
                ostringstream oss;
                oss << "[" << username << "] Entered invalid menu option: " << ch;
                logger.warn(oss.str());
                break;
            }
        }
    }
}
