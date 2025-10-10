#include <iostream>
#include <string>
#include <sstream>
#include "ManagerMenu.h"
#include "../Logger/LoggerGlobal.h"

using namespace std;

void ManagerMenu::show(const string& username, AuthManager& auth, ZooGraph& zoo) {
    bool running = true;
    while (running) {
        cout << "\n===== Manager Menu (logged as " << username << ") =====\n"
                "1.  Assign employee to aviary\n"
                "2.  Reassign employee between aviaries\n"
                "3.  Remove employee from aviary\n"
                "4.  Add animal to aviary\n"
                "5.  Move animal between aviaries\n"
                "6.  Remove animal from aviary\n"
                "7.  View unassigned employees\n"
                "8.  View unassigned animals\n"
                "9.  List all employees\n"
                "10. List all animals\n"
                "11. List all aviaries\n"
                "12. View neighbors of aviary\n"
                "13. Find shortest path between aviaries\n"
                "14. Calculate distance between aviaries\n"
                "15. Activate log\n"
                "16. Return to login screen\n"
                "========================================================\n"
                "Choice: ";

        int ch;
        cin >> ch;

        switch (ch) {
            case 1: {
                string empId, aviaryId;
                cout << "Employee ID: "; cin >> empId;
                cout << "Aviary ID: "; cin >> aviaryId;

                {
                    ostringstream oss;
                    oss << "[" << username << "] Assign employee " << empId << " to aviary " << aviaryId;
                    logger.info(oss.str());
                }

                if (zoo.getEmployeeManager().assignEmployeeToAviary(empId, aviaryId)) {
                    cout << "Employee assigned successfully.\n";
                    ostringstream oss;
                    oss << "[" << username << "] Employee " << empId << " assigned successfully.";
                    logger.info(oss.str());
                } else {
                    cout << "Failed to assign employee.\n";
                    ostringstream oss;
                    oss << "[" << username << "] Failed to assign employee " << empId;
                    logger.warn(oss.str());
                }
                break;
            }

            case 2: {
                string empId, fromId, toId;
                cout << "Employee ID: "; cin >> empId;
                cout << "From Aviary ID: "; cin >> fromId;
                cout << "To Aviary ID: "; cin >> toId;

                {
                    ostringstream oss;
                    oss << "[" << username << "] Reassign employee " << empId << " from " << fromId << " to " << toId;
                    logger.info(oss.str());
                }

                if (zoo.getEmployeeManager().reassignEmployee(empId, fromId, toId)) {
                    cout << "Employee reassigned successfully.\n";
                    ostringstream oss;
                    oss << "[" << username << "] Employee " << empId << " reassigned successfully.";
                    logger.info(oss.str());
                } else {
                    cout << "Failed to reassign employee.\n";
                    ostringstream oss;
                    oss << "[" << username << "] Failed to reassign employee " << empId;
                    logger.warn(oss.str());
                }
                break;
            }

            case 3: {
                string empId, avId;
                cout << "Employee ID to remove: "; cin >> empId;
                cout << "Aviary ID: "; cin >> avId;

                {
                    ostringstream oss;
                    oss << "[" << username << "] Removing employee " << empId << " from aviary " << avId;
                    logger.info(oss.str());
                }

                if (zoo.getEmployeeManager().removeEmployeeFromAviary(empId, avId)) {
                    cout << "Employee removed.\n";
                    ostringstream oss;
                    oss << "[" << username << "] Employee " << empId << " removed from aviary.";
                    logger.info(oss.str());
                } else {
                    cout << "Failed to remove employee.\n";
                    ostringstream oss;
                    oss << "[" << username << "] Failed to remove employee " << empId;
                    logger.warn(oss.str());
                }
                break;
            }

            case 4: {
                string animalId, aviaryId;
                cout << "Animal ID: "; cin >> animalId;
                cout << "Aviary ID: "; cin >> aviaryId;

                {
                    ostringstream oss;
                    oss << "[" << username << "] Adding animal " << animalId << " to aviary " << aviaryId;
                    logger.info(oss.str());
                }

                if (zoo.getAnimalManager().addAnimalInAviary(aviaryId, animalId)) {
                    cout << "Animal added successfully.\n";
                    ostringstream oss;
                    oss << "[" << username << "] Animal " << animalId << " added successfully.";
                    logger.info(oss.str());
                } else {
                    cout << "Failed to add animal.\n";
                    ostringstream oss;
                    oss << "[" << username << "] Failed to add animal " << animalId;
                    logger.warn(oss.str());
                }
                break;
            }

            case 5: {
                string animalId, fromId, toId;
                cout << "Animal ID: "; cin >> animalId;
                cout << "From Aviary ID: "; cin >> fromId;
                cout << "To Aviary ID: "; cin >> toId;

                {
                    ostringstream oss;
                    oss << "[" << username << "] Moving animal " << animalId << " from " << fromId << " to " << toId;
                    logger.info(oss.str());
                }

                if (zoo.getAnimalManager().moveAnimalBetweenAviaries(fromId, toId, animalId)) {
                    cout << "Animal moved successfully.\n";
                    ostringstream oss;
                    oss << "[" << username << "] Animal " << animalId << " moved successfully.";
                    logger.info(oss.str());
                } else {
                    cout << "Failed to move animal.\n";
                    ostringstream oss;
                    oss << "[" << username << "] Failed to move animal " << animalId;
                    logger.warn(oss.str());
                }
                break;
            }

            case 6: {
                string animalId, aviaryId;
                cout << "Animal ID: "; cin >> animalId;
                cout << "Aviary ID: "; cin >> aviaryId;

                {
                    ostringstream oss;
                    oss << "[" << username << "] Removing animal " << animalId << " from aviary " << aviaryId;
                    logger.info(oss.str());
                }

                if (zoo.getAnimalManager().removeAnimalFromAviary(aviaryId, animalId)) {
                    cout << "Animal removed successfully.\n";
                    ostringstream oss;
                    oss << "[" << username << "] Animal " << animalId << " removed successfully.";
                    logger.info(oss.str());
                } else {
                    cout << "Failed to remove animal.\n";
                    ostringstream oss;
                    oss << "[" << username << "] Failed to remove animal " << animalId;
                    logger.warn(oss.str());
                }
                break;
            }

            case 7: {
                {
                    ostringstream oss;
                    oss << "[" << username << "] Listing unassigned employees";
                    logger.info(oss.str());
                }
                auto unassigned = zoo.getEmployeeManager().getUnassignedEmployees();
                cout << "--- Employees not assigned to any aviary ---\n";
                for (auto& a : unassigned)
                    cout << "• " << a->getName() << " (ID: " << a->getId() << ")\n";
                if (unassigned.empty())
                    cout << "All employees are assigned.\n";
                break;
            }

            case 8: {
                {
                    ostringstream oss;
                    oss << "[" << username << "] Listing unassigned animals";
                    logger.info(oss.str());
                }
                auto unassigned = zoo.getAnimalManager().getAnimalsNotInAviaries();
                cout << "--- Animals not assigned to any aviary ---\n";
                for (auto& a : unassigned)
                    cout << "• " << a->getName() << " (ID: " << a->getId() << ")\n";
                if (unassigned.empty())
                    cout << "All animals are assigned.\n";
                break;
            }

            case 9:
                {
                    ostringstream oss;
                    oss << "[" << username << "] Listing all employees";
                    logger.info(oss.str());
                }
                zoo.listEmployees();
                break;

            case 10:
                {
                    ostringstream oss;
                    oss << "[" << username << "] Listing all animals";
                    logger.info(oss.str());
                }
                zoo.listAnimals();
                break;

            case 11:
                {
                    ostringstream oss;
                    oss << "[" << username << "] Listing all aviaries";
                    logger.info(oss.str());
                }
                zoo.printAviaries();
                break;

            case 12: {
                string aviaryId;
                cout << "Enter Aviary ID: "; cin >> aviaryId;
                {
                    ostringstream oss;
                    oss << "[" << username << "] Viewing neighbors of aviary " << aviaryId;
                    logger.info(oss.str());
                }
                auto neighbors = zoo.getNeighborsNames(aviaryId);
                cout << "Neighbors of " << aviaryId << ":\n";
                for (auto& n : neighbors) cout << "• " << n << "\n";
                break;
            }

            case 13: {
                string startId, endId;
                cout << "Start Aviary ID: "; cin >> startId;
                cout << "End Aviary ID: "; cin >> endId;
                {
                    ostringstream oss;
                    oss << "[" << username << "] Finding shortest path between " << startId << " and " << endId;
                    logger.info(oss.str());
                }
                auto path = zoo.findShortestPath(startId, endId);
                cout << "Shortest path: ";
                for (auto& p : path) cout << p << " -> ";
                cout << "END\n";
                break;
            }

            case 14: {
                string fromId, toId;
                cout << "From Aviary ID: "; cin >> fromId;
                cout << "To Aviary ID: "; cin >> toId;
                {
                    ostringstream oss;
                    oss << "[" << username << "] Calculating distance between " << fromId << " and " << toId;
                    logger.info(oss.str());
                }
                double dist = zoo.distanceBetweenAviaries(fromId, toId);
                cout << "Distance: " << dist << "\n";
                break;
            }

            case 15: {
                {
                    ostringstream oss;
                    oss << "[" << username << "] Logger configuration opened";
                    logger.info(oss.str());
                }
                logger.listEnabledLevels();
                cout << "1. Enable Level\n2. Disable Level\nAny button to move back \nChoice: ";
                int ch2; cin >> ch2;
                switch (ch2) {
                    case 1: {
                        cout << "1. Enable INFO\n2. Enable WARN\n3. Enable ERROR\nChoice: ";
                        int ch3; cin >> ch3;
                        if (ch3 == 1) logger.enable(Logger::INFO);
                        else if (ch3 == 2) logger.enable(Logger::WARN);
                        else if (ch3 == 3) logger.enable(Logger::ERROR);
                        else cout << "Wrong choice\n";
                        break;
                    }
                    case 2: {
                        cout << "1. Disable INFO\n2. Disable WARN\n3. Disable ERROR\nChoice: ";
                        int ch3; cin >> ch3;
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

            case 16:
                {
                    ostringstream oss;
                    oss << "[" << username << "] Closed ManagerMenu";
                    logger.info(oss.str());
                }
                running = false;
                break;

            default:
                cout << "Wrong choice. Try again.\n";
                {
                    ostringstream oss;
                    oss << "[" << username << "] Invalid choice";
                    logger.warn(oss.str());
                }
            break;
        }
    }
}
