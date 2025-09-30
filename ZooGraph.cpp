#include "ZooGraph.h"
#include <iostream>
#include <algorithm>

void ZooGraph::addEnclosure(string name) {
    if (adjList.find(name) == adjList.end()) {
        adjList[name] = {};
        cout << "Added enclosure: " << name << endl;
    }
}

void ZooGraph::removeEnclosure(string name) {
    if (adjList.find(name) != adjList.end()) {
        adjList.erase(name);
        employeesInEnclosure.erase(name);
        for (auto &p : adjList) {
            auto &vec = p.second;
            vec.erase(remove(vec.begin(), vec.end(), name), vec.end());
        }
        cout << "Removed enclosure: " << name << endl;
    }
}

void ZooGraph::addPath(string from, string to) {
    if (adjList.find(from) != adjList.end() && adjList.find(to) != adjList.end()) {
        adjList[from].push_back(to);
        adjList[to].push_back(from);
        cout << "Added path between " << from << " and " << to << endl;
    }
}

void ZooGraph::removePath(string from, string to) {
    if (adjList.find(from) != adjList.end() && adjList.find(to) != adjList.end()) {
        auto &vec1 = adjList[from];
        auto &vec2 = adjList[to];
        vec1.erase(remove(vec1.begin(), vec1.end(), to), vec1.end());
        vec2.erase(remove(vec2.begin(), vec2.end(), from), vec2.end());
        cout << "Removed path between " << from << " and " << to << endl;
    }
}

void ZooGraph::assignEmployee(string enclosure, Employee e) {
    if (adjList.find(enclosure) != adjList.end()) {
        employeesInEnclosure[enclosure].push_back(e);
        cout << "Assigned " << e.getName() << " to " << enclosure << endl;
    }
}

void ZooGraph::printGraph() {
    cout << "\nZoo Enclosures and Paths:" << endl;
    for (auto &p : adjList) {
        cout << p.first << ": ";
        for (auto &neighbor : p.second) {
            cout << neighbor << " ";
        }
        cout << endl;
    }
}

void ZooGraph::printEmployees() {
    cout << "\nEmployees in enclosures:" << endl;
    for (auto &p : employeesInEnclosure) {
        cout << p.first << ": " << endl;
        for (auto &emp : p.second) {
            cout << "   " << emp.getFullInfo();
        }
    }
}
