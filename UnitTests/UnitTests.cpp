#include "UnitTests.h"

void AccountTests::test_constructor_getters() {
    Account acc("user", 12345, Role::MANAGER);
    assert(acc.getUsername() == "user" && "Username should match constructor value");
    assert(acc.getPasswordHash() == 12345 && "Password hash should match constructor value");
    assert(acc.getRole() == Role::MANAGER && "Role should match constructor value");
}
void AccountTests::test_role_conversions() {
    assert(Account::roleToString(Role::ADMIN) == "Admin");
    assert(Account::roleToString(Role::MANAGER) == "Manager");
    assert(Account::roleToString(Role::EMPLOYEE) == "Employee");
    assert(Account::roleToInt(Role::ADMIN) == 0);
    assert(Account::roleToInt(Role::MANAGER) == 1);
    assert(Account::roleToInt(Role::EMPLOYEE) == 2);
    assert(Account::intToRole(0) == Role::ADMIN);
    assert(Account::intToRole(1) == Role::MANAGER);
    assert(Account::intToRole(2) == Role::EMPLOYEE);
}
void AccountTests::test_serialize_deserialize() {
    Account a("bob", 9876, Role::EMPLOYEE);
    string line = a.serialize();
    Account b = Account::deserialize(line);
    assert(b.getUsername() == "bob");
    assert(b.getPasswordHash() == 9876);
    assert(b.getRole() == Role::EMPLOYEE);
}

void AccountTests::run() {
    test_constructor_getters();
    test_role_conversions();
    test_serialize_deserialize();
}

void AuthManagerTests::test_add_remove_list_accounts() {
    DatabaseManager db(":memory:");
    AccountRepository repo(db);
    repo.initTable();
    AuthManager auth(repo);
    // Default accounts should exist
    assert(auth.accountExists("admin"));
    assert(auth.accountExists("manager"));
    assert(auth.accountExists("employee"));
    // Add new account
    bool added = auth.addAccount("user1", "pass", Role::EMPLOYEE);
    assert(added);
    assert(auth.accountExists("user1"));
    vector<string> names = auth.listUsernames();
    bool found = false;
    for (auto& n : names) {
        if (n == "user1") { found = true; break; }
    }
    assert(found && "New user should be in listUsernames");
    // Remove account
    bool removed = auth.removeAccount("user1");
    assert(removed);
    assert(!auth.accountExists("user1"));
    // Remove again should fail
    assert(auth.removeAccount("user1") == false);
    // Adding existing (admin) should fail
    assert(auth.addAccount("admin", "xx", Role::ADMIN) == false);
    assert(auth.accountExists("admin"));
}

void AuthManagerTests::run() {
    test_add_remove_list_accounts();
}

void AnimalTests::test_feed_and_compatibility() {
    auto m1 = make_shared<Mammal>("Dog","Labrador",5,30.0,"Mammal");
    auto m2 = make_shared<Mammal>("Cat","Siamese",3,5.0,"Mammal");
    assert(m1->isCompatibleWith(m2) && "Different mammals should be compatible");
    auto lion = make_shared<Mammal>("Lion","Lion",4,190.0,"Mammal");
    auto tiger = make_shared<Mammal>("Tiger","Tiger",3,220.0,"Mammal");
    assert(!lion->isCompatibleWith(tiger) && "Lion and Tiger should not be compatible");
    auto eagle = make_shared<Bird>("Eag","Eagle",2,5.0,"Bird");
    auto parrot = make_shared<Bird>("Par","Parrot",1,0.1,"Bird");
    assert(!eagle->isCompatibleWith(parrot) && "Eagle and Parrot should not be compatible");
    auto snake = make_shared<Reptile>("Snk","Snake",2,50.0,"Reptile");
    assert(!snake->isCompatibleWith(m1) && "Snake and Mammal should not be compatible");
    auto frog = make_shared<Amphibian>("Frog","Frog",1,0.5,"Amphibian");
    auto fly = make_shared<Insect>("Fly","Dragonfly",1,0.01,"Insect");
    assert(!frog->isCompatibleWith(fly) && "Amphibian and Insect should not be compatible");
    auto spider = make_shared<Arachnid>("Ara","Spider",1,0.1,"Arachnid");
    auto gold  = make_shared<Fish>("Gold","Goldfish",1,0.1,"Fish");
    assert(!spider->isCompatibleWith(gold) && "Arachnid and Fish should not be compatible");
    auto animal = make_shared<Mammal>("Bun","Bunny",1,2.0,"Mammal");
    assert(!animal->getIsFed());
    animal->feed();
    assert(animal->getIsFed());
    animal->feed();
    assert(animal->getIsFed());
}

void AnimalTests::run() {
    test_feed_and_compatibility();
}

void GraphTests::test_connectivity_and_paths() {
    Graph g;
    assert(g.checkConnectivity());
    auto v1 = make_shared<Vertex>("A");
    auto v2 = make_shared<Vertex>("B");
    auto v3 = make_shared<Vertex>("C");
    g.addVertex(v1);
    g.addVertex(v2);
    assert(!g.checkConnectivity());
    g.addEdge(v1->getId(), v2->getId(), 1.5);
    assert(g.checkConnectivity());
    vector<string> path = g.findPath(v1->getId(), v2->getId());
    assert(path.size() == 2);
    assert(path[0] == v1->getId() && path[1] == v2->getId());
    assert(g.distanceBetween(v1->getId(), v2->getId()) == 1.5);
    g.addVertex(v3);
    g.addEdge(v2->getId(), v3->getId(), 2.0);
    assert(g.checkConnectivity());
    vector<string> path2 = g.findPath(v1->getId(), v3->getId());
    assert(path2.size() == 3);
    double dist = g.distanceBetween(v1->getId(), v3->getId());
    assert(dist == 3.5);
}

void GraphTests::run() {
    test_connectivity_and_paths();
}

 void LoggerTests::test_enable_disable() {
    assert(logger.isEnabled(Logger::INFO));
    logger.disable(Logger::INFO);
    assert(!logger.isEnabled(Logger::INFO));
    logger.enable(Logger::INFO);
    assert(logger.isEnabled(Logger::INFO));
}

void LoggerTests::run() {
    test_enable_disable();
}
