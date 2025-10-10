#include "Menu.h"
#include "AdminMenu.h"
#include "ManagerMenu.h"
#include "../Logger/LoggerGlobal.h"
#include <iostream>
#include <sstream>

using namespace std;

void Menu::showFor(const string& username, Role role, AuthManager& auth, ZooGraph& zoo) {
    switch (role) {
        case Role::ADMIN: {
            ostringstream oss;
            oss << "[" << username << "] Opened AdminMenu";
            logger.info(oss.str());
            AdminMenu::show(username, auth, zoo);
            break;
        }

        case Role::MANAGER: {
            ostringstream oss;
            oss << "[" << username << "] Opened ManagerMenu";
            logger.info(oss.str());
            ManagerMenu::show(username, auth, zoo);
            break;
        }

        case Role::EMPLOYEE: {
            ostringstream oss;
            oss << "[" << username << "] Opened EmployeeMenu";
            logger.info(oss.str());

            bool running = true;
            while (running) {
                cout << "\n--- Employee Menu (logged as " << username << ") ---\n"
                     << "1. View your aviaries\n"
                     << "2. View animals in your aviaries\n"
                     << "3. Feed an animal by ID\n"
                     << "4. Find shortest path between aviaries\n"
                     << "5. Calculate distance between aviaries\n"
                     << "6. Activate log\n"
                     << "7. Return to login screen\n"
                     << "Choice: ";

                int ch;
                cin >> ch;

                switch (ch) {
                    case 1: {
                        string id;
                        cout << "Enter your ID: ";
                        cin >> id;

                        ostringstream oss;
                        oss << "[" << username << "] Requested to view own aviaries (ID: " << id << ")";
                        logger.info(oss.str());

                        auto emp = zoo.getEmployeeManager().getEmployee(id);
                        if (emp) {
                            cout << "Your aviaries:\n" << emp->listAviaries() << endl;
                        } else {
                            cout << "Employee not found.\n";
                            ostringstream oss2;
                            oss2 << "[" << username << "] Employee with ID " << id << " not found.";
                            logger.warn(oss2.str());
                        }
                        break;
                    }

                    case 2: {
                        string id;
                        cout << "Enter your ID: ";
                        cin >> id;

                        ostringstream oss;
                        oss << "[" << username << "] Requested to view animals in aviaries (ID: " << id << ")";
                        logger.info(oss.str());

                        auto emp = zoo.getEmployeeManager().getEmployee(id);
                        if (emp) {
                            for (const auto& aviaryId : emp->getAviaryIds()) {
                                auto aviary = dynamic_pointer_cast<Aviary>(zoo.getAviaryById(aviaryId));
                                if (aviary) {
                                    cout << "Aviary: " << aviary->getName() << endl;
                                    aviary->listAnimals();
                                }
                            }
                        } else {
                            cout << "Employee not found.\n";
                            ostringstream oss2;
                            oss2 << "[" << username << "] Employee with ID " << id << " not found.";
                            logger.warn(oss2.str());
                        }
                        break;
                    }

                    case 3: {
                        string animalId;
                        cout << "Enter Animal ID to feed: ";
                        cin >> animalId;

                        ostringstream oss;
                        oss << "[" << username << "] Feeding animal with ID " << animalId;
                        logger.info(oss.str());

                        zoo.getAnimalManager().feedById(animalId);
                        cout << "Feeding command executed.\n";
                        break;
                    }

                    case 4: {
                        string startId, endId;
                        cout << "Start Aviary ID: ";
                        cin >> startId;
                        cout << "End Aviary ID: ";
                        cin >> endId;

                        ostringstream oss;
                        oss << "[" << username << "] Finding shortest path between " << startId << " and " << endId;
                        logger.info(oss.str());

                        auto path = zoo.findShortestPath(startId, endId);
                        cout << "Shortest path: ";
                        for (auto& p : path) cout << p << " -> ";
                        cout << "END\n";
                        break;
                    }

                    case 5: {
                        string fromId, toId;
                        cout << "From Aviary ID: ";
                        cin >> fromId;
                        cout << "To Aviary ID: ";
                        cin >> toId;

                        ostringstream oss;
                        oss << "[" << username << "] Calculating distance between " << fromId << " and " << toId;
                        logger.info(oss.str());

                        double dist = zoo.distanceBetweenAviaries(fromId, toId);
                        cout << "Distance: " << dist << "\n";
                        break;
                    }

                    case 6: {
                        ostringstream oss;
                        oss << "[" << username << "] Opened logger configuration menu";
                        logger.info(oss.str());

                        logger.listEnabledLevels();
                        cout << "1. Enable Level\n2. Disable Level\nAny button to move back \nChoice: ";
                        int ch2;
                        cin >> ch2;
                        switch (ch2) {
                            case 1: {
                                cout << "1. Enable INFO\n2. Enable WARN\n3. Enable ERROR\nChoice: ";
                                int ch3;
                                cin >> ch3;
                                if (ch3 == 1) logger.enable(Logger::INFO);
                                else if (ch3 == 2) logger.enable(Logger::WARN);
                                else if (ch3 == 3) logger.enable(Logger::ERROR);
                                else cout << "Wrong choice\n";
                                break;
                            }
                            case 2: {
                                cout << "1. Disable INFO\n2. Disable WARN\n3. Disable ERROR\nChoice: ";
                                int ch3;
                                cin >> ch3;
                                if (ch3 == 1) logger.disable(Logger::INFO);
                                else if (ch3 == 2) logger.disable(Logger::WARN);
                                else if (ch3 == 3) logger.disable(Logger::ERROR);
                                else cout << "Wrong choice\n";
                                break;
                            }
                            default: cout << "Wrong choice\n";
                        }
                        break;
                    }

                    case 7: {
                        ostringstream oss;
                        oss << "[" << username << "] Logged out from EmployeeMenu";
                        logger.info(oss.str());
                        running = false;
                        break;
                    }

                    default: {
                        cout << "Wrong choice\n";
                        ostringstream oss;
                        oss << "[" << username << "] Invalid menu choice (" << ch << ")";
                        logger.warn(oss.str());
                        break;
                    }
                }
            }
            break;
        }
    }
}
