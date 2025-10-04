#include "Graph.h"
#include "ZooGraph.h"
#include <iostream>
#include <set>
#include <utility>
 using namespace std;

string getAviaryName(const ZooGraph& zoo, const string& uuid) {
    auto it = zoo.getVertices().find(uuid);
    if (it == zoo.getVertices().end()) return "";

    auto aviary = std::dynamic_pointer_cast<Aviary>(it->second);
    if (!aviary) return "";

    return aviary->getName();
}

//===========Aviary===========
void Aviary::printInfo() const {
    cout << "Aviary [" << getId() << "]"
         << " Name: " << name
         << ", Type: " << type
         << ", Area: " << area << " m^2" << endl;
}

string Aviary::getName() const { return name; }
string Aviary::getType() const { return type; }
double Aviary::getArea() const { return area; }

void Aviary::setName(const string& n) { name = n; }
void Aviary::setType(const string& t) { type = t; }
void Aviary::setArea(double a) { area = a; }

//===========Path===========
double Path::getLength() const { return getWeight(); }

//===========ZooGraph===========
string ZooGraph::getAviaryNameById(const std::string& id) const {
    auto it = vertices.find(id);
    if (it == vertices.end()) return "";

    auto aviary = dynamic_pointer_cast<Aviary>(it->second);
    if (!aviary) return "";

    return aviary->getName();
}

void ZooGraph::addAviary(shared_ptr<Aviary> aviary) {
    addVertex(aviary);
}

void ZooGraph::removeAviary(const string& id) {
    removeVertex(id);
}

void ZooGraph::addPath(const string& fromId, const string& toId, double length) {
    addEdge(fromId, toId, length);
}

void ZooGraph::removePath(const string& fromId, const string& toId) {
    removeEdge(fromId, toId);
}

vector<string> ZooGraph::findShortestPath(const string& startId, const string& endId) const {
    return findPathByWeight(startId, endId);
}

double ZooGraph::distanceBetweenAviaries(const std::string& fromId, const std::string& toId) const{
    return distanceBetween(fromId, toId);
}

bool ZooGraph::isZooConnected() const {
    return checkConnectivity();
}

void ZooGraph::printPathBetweenAviaries(const std::string& fromId, const std::string& toId) const {
    vector<string> path = findPathByWeight(fromId, toId);

    if (path.empty()) {
        cout << "No path between aviaries!" << endl;
        return;
    }

    cout << "Path: ";
    for (size_t i = 0; i < path.size(); ++i) {
        cout << getAviaryNameById(path[i]);
        if (i + 1 < path.size()) cout << " -> ";
    }
    cout << endl;

    double totalDist = distanceBetween(path.front(), path.back());
    cout << "Total distance: " << totalDist << " m" << endl;
}

void ZooGraph::printAviaries() const {
    cout << "=======================================\n";
    cout << "Aviaries:\n";
    for (const auto& [id, aviary] : vertices) {
        auto av = dynamic_pointer_cast<Aviary>(aviary);
        if (av) av->printInfo();
    }
    cout << "=======================================\n";
}

void ZooGraph::printZoo() const {
    cout << "=======================================\n";
    cout << "Zoo structure:\n";
    for (const auto& [id, aviary] : vertices) {
        auto av = dynamic_pointer_cast<Aviary>(aviary);
        if (av) av->printInfo();
    }

    cout << "\nPaths:\n";

    set<pair<string,string>> printed;
    for (const auto& e : edges) {
        string u = e.getFrom();
        string v = e.getTo();

        pair p = { min(u,v), max(u,v) };

        if (printed.count(p)) continue;

        cout << getAviaryNameById(u)<< " <--(" << e.getWeight() << " m)--> "<< getAviaryNameById(v)<< endl;

        printed.insert(p);
    }
        cout << "=======================================\n";

}
