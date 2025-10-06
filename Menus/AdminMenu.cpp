#include "AdminMenu.h"
#include <iostream>
using namespace std;

void AdminMenu::show(const string& username, AuthManager& auth, ZooGraph& zoo) {
    bool running = true;
    while (running) {
        cout << "\n--- Admin Menu (logged as " << username << ") ---\n";
        cout << "1. Create account\n2. Delete account\n3. Edit account\n";
        cout << "4. Create animal\n5. Delete animal\n";
        cout << "6. Create employee\n7. Delete employee\n";
        cout << "8. Create aviary\n9. Delete aviary\n";
        //cout << "10. View action log\n";
        cout << "5. Return to the login screen\n";
        cout << "6. Edit account\n";
        cout << "Choice: ";
        int ch; cin >> ch;


        /*case 10: {
            // ifstream f("log.txt");
            // if (!f) { cout << "Лог не знайдено.\n"; break; }
            // cout << f.rdbuf();
            break;
        }*/

        switch (ch) {
            case 1: {
                string u, p; int r;
                cout << "New login: "; cin >> u;
                cout << "Password: "; cin >> p;
                cout << "Role (0=Admin,1=Manager,2=Employee): "; cin >> r;
                Role role = Account::intToRole(r);
                if (auth.addAccount(u, p, role)) {
                    cout << "Account added.\n";
                    // Logger::logAction(username, "Added account " + u);
                } else {
                    cout << "An account with that name already exists.\n";
                }
                break;
            }
            case 2: {
                string u;
                cout << "Login to delete: "; cin >> u;
                if (auth.removeAccount(u)) {
                    cout << "Account deleted.\n";
                    // Logger::logAction(username, "Removed account " + u);
                } else {
                    cout << "Account not found.\n";
                }
                break;
            }
            case 3: {
                string u;
                cout << "Enter account login to edit: ";
                cin >> u;
                auth.editAccount(u);
                break;
            }
            case 4: {
                int age;
                double weight;
                string name, species, type;
                cout << "Name: "; cin >> name;
                cout << "Species: "; cin >> species;
                cout << "Age: "; cin >> age;
                cout << "Weight: "; cin >> weight;
                cout << "Type: "; cin >> type;
                zoo.getAnimalManager().createAnimal(name, species, age, weight, type);
                break;
            }
            case 5: {
                string id;
                cout << "Animal ID to delete: "; cin >> id;
                zoo.getAnimalManager().removeAnimalFromAnimals(id);
                break;
            }
            case 6: {
                string name;
                int salary, age, experience;
                cout << "Name: "; cin >> name;
                cout << "Salary: "; cin >> salary;
                cout << "Age: "; cin >> age;
                cout << "Experience: "; cin >> experience;
                zoo.getEmployeeManager().addEmployee(make_shared<Employee>(name, age, salary, experience));
                break;
            }
            case 7: {
                string id;
                cout << "Employee ID to delete: "; cin >> id;
                zoo.getEmployeeManager().removeEmployee(id);
                break;
            }


            default:
                cout << "Wrong choice\n";
        }
    }
}

/*
void AdminMenu::show(const string& username, AuthManager& auth, ZooGraph& zoo) {
    bool running = true;
    while (running) {
        cout << "\n--- Admin Menu (logged as " << username << ") ---\n";
        cout << "1. Create account\n2. Delete account\n3. Edit account\n";
        cout << "4. Create animal\n5. Delete animal\n";
        cout << "6. Create employee\n7. Delete employee\n";
        cout << "8. Create aviary\n9. Delete aviary\n";
        cout << "10. List all animals\n11. List all employees\n12. List all aviaries\n";
        cout << "13. View action log\n14. Return\nChoice: ";
        int ch; cin >> ch;

        switch(ch) {
            case 1: { // create account
                string u, p; int r;
                cout << "Login: "; cin >> u;
                cout << "Password: "; cin >> p;
                cout << "Role (0=Admin,1=Manager,2=Employee): "; cin >> r;
                Role role = Account::intToRole(r);
                if(auth.addAccount(u,p,role)) cout << "Account added.\n";
                else cout << "Account exists.\n";
                break;
            }
            case 2: { string u; cout << "Login to delete: "; cin >> u;
                if(auth.removeAccount(u)) cout << "Deleted.\n"; else cout << "Not found.\n"; break;}
            case 3: { string u; cout << "Login to edit: "; cin >> u; auth.editAccount(u); break; }
            case 4: { // create animal
                string id, name, species;
                cout << "Animal ID: "; cin >> id;
                cout << "Name: "; cin >> name;
                cout << "Species: "; cin >> species;
                zoo.getAnimalManager().addAnimal(id,name,species);
                break;
            }
            case 5: { string id; cout << "Animal ID to delete: "; cin >> id;
                zoo.getAnimalManager().removeAnimal(id); break; }
            case 6: { // create employee
                string id,name; 
                cout << "Employee ID: "; cin >> id;
                cout << "Name: "; cin >> name;
                zoo.getEmployeeManager().addEmployee(id,name);
                break;
            }
            case 7: { string id; cout << "Employee ID to delete: "; cin >> id;
                zoo.getEmployeeManager().removeEmployee(id); break;}
            case 8: { string id,name; cout << "Aviary ID: "; cin >> id;
                cout << "Name: "; cin >> name;
                zoo.addAviary(id,name); break;}
            case 9: { string id; cout << "Aviary ID to delete: "; cin >> id;
                zoo.removeAviary(id); break;}
            case 10: zoo.listAnimals(); break;
            case 11: zoo.listEmployees(); break;
            case 12: zoo.listAviaries(); break;
            case 13: zoo.showLogs(); break;
            case 14: running=false; break;
            default: cout << "Wrong choice\n";
        }
    }
}
*/
