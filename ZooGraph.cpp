#include "ZooGraph.h"
#include <iostream>
 using namespace std;

//===========Aviary===========
void Aviary::printInfo() const {
    cout << "Aviary [" << getId() << "]"
         << " Name: " << name
         << ", Type: " << type
         << ", Area: " << area << " m²" << endl;
}

string Aviary::getName() const { return name; }
string Aviary::getType() const { return type; }
double Aviary::getArea() const { return area; }

void Aviary::setName(const string& n) { name = n; }
void Aviary::setType(const string& t) { type = t; }
void Aviary::setArea(double a) { area = a; }

//===========Path===========
int Path::getLength() const { return getWeight(); }

//===========ZooGraph===========
void ZooGraph::addAviary(shared_ptr<Aviary> aviary) {
    addVertex(aviary);
}

void ZooGraph::removeAviary(const string& id) {
    removeVertex(id);
}

void ZooGraph::addPath(const string& fromId, const string& toId, int length) {
    addEdge(fromId, toId, length);
}

void ZooGraph::removePath(const string& fromId, const string& toId) {
    removeEdge(fromId, toId);
}

void ZooGraph::printZoo() const {
    cout << "Zoo structure:\n";
    for (const auto& [id, aviary] : vertices) {
        auto av = dynamic_pointer_cast<Aviary>(aviary);
        if (av) av->printInfo();
    }

    cout << "\nPaths:\n";
    for (const auto& e : edges) {
        cout << e.getFrom() << " --(" << e.getWeight() << " m)--> " << e.getTo() << endl;
    }
}