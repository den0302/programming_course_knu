#include "Graph.h"
#include <iostream>
#include <vector>
#define byte win_byte_override
#include <windows.h>
#undef byte
#include <rpcdce.h>
#include <memory>
#include <string>
#include <algorithm>
#include <queue>
#include <limits>
using namespace std;

//===========Vertex===========
Vertex::Vertex() {
    UUID uuid;
    RPC_STATUS status = UuidCreate(&uuid);
    if (status != RPC_S_OK && status != RPC_S_UUID_LOCAL_ONLY && status != RPC_S_UUID_NO_ADDRESS) {
        throw runtime_error("UuidCreate failed, RPC_STATUS = " + to_string(status));
    }

    RPC_CSTR strUuid = nullptr;
    status = UuidToStringA(&uuid, &strUuid);
    if (status != RPC_S_OK) {
        throw runtime_error("UuidToString failed, RPC_STATUS = " + to_string(status));
    }

    id = string(reinterpret_cast<char*>(strUuid));
    RpcStringFreeA(&strUuid);
}

Vertex::~Vertex() = default;

string Vertex::getId() const { return id; }

//===========Edge===========
string Edge::getFrom() const { return fromId; }
string Edge::getTo() const { return toId; }
double Edge::getWeight() const { return weight; }

//===========Graph===========
const unordered_map<string, shared_ptr<Vertex>>& Graph::getVertices() const { return vertices; }

vector<string> Graph::getNeighbors(const string& id) const {
    vector<string> neighbors;

    for (const auto& e : edges) {
        if (e.getFrom() == id) {
            neighbors.push_back(e.getTo());
        } else if (e.getTo() == id) {
            neighbors.push_back(e.getFrom());
        }
    }

    return neighbors;
}

void Graph::setVertices(const unordered_map<string, shared_ptr<Vertex>>& newVertices) {
    vertices = newVertices;
}

void Graph::addVertex(shared_ptr<Vertex> v) {
    vertices[v->getId()] = v;
}

shared_ptr<Vertex> Graph::getVertex(const string& id) const {
    auto it = vertices.find(id);
    return (it != vertices.end()) ? it->second : nullptr;
}

const Edge* Graph::getEdge(const string& fromId, const string& toId) const {
    for (const auto& e : edges) {
        if ((e.getFrom() == fromId && e.getTo() == toId) ||
            (e.getFrom() == toId && e.getTo() == fromId)) {
            return &e; // повертаємо вказівник на знайдене ребро
            }
    }
    return nullptr; // якщо не знайдено
}


void Graph::removeVertex(const string& id) {
    if (!vertices.count(id)) {
        cout << "Vertex with id " << id << " not found!" << endl;
        return;
    }
    vertices.erase(id);
    edges.erase(remove_if(edges.begin(), edges.end(),
                          [&](const Edge& e) {
                              return e.getFrom() == id || e.getTo() == id;
                          }),
                edges.end());
}

const vector<Edge>& Graph::getEdges() const {
    return edges;
}

void Graph::setEdges(const vector<Edge>& newEdges) {
    edges = newEdges;
}

void Graph::addEdge(const string& fromId, const string& toId, double weight = 1.0) {
    if (vertices.count(fromId) && vertices.count(toId)) {
        edges.emplace_back(fromId, toId, weight);
        edges.emplace_back(toId, fromId, weight);
    } else {
        cout << "Error: one or both vertices not found!" << endl;
    }
}

void Graph::removeEdge(const string& fromId, const string& toId) {
    edges.erase(remove_if(edges.begin(), edges.end(),
                          [&](const Edge& e) {
                              return (e.getFrom() == fromId && e.getTo() == toId) ||
                                  (e.getFrom() == toId && e.getTo() == fromId);
                          }),
                edges.end());
}

vector<string> Graph::findPath(const string& startId, const string& endId) {
    unordered_map<string, string> parent;
    queue<string> q;
    unordered_map<string, bool> visited;

    q.push(startId);
    visited[startId] = true;

    while (!q.empty()) {
        string cur = q.front(); q.pop();

        if (cur == endId) break;

        for (const auto& e : edges) {
            string neighbor;
            if (e.getFrom() == cur) neighbor = e.getTo();
            else if (e.getTo() == cur) neighbor = e.getFrom();
            else continue;

            if (!visited[neighbor]) {
                visited[neighbor] = true;
                parent[neighbor] = cur;
                q.push(neighbor);
            }
        }
    }

    vector<string> path;
    if (!visited[endId]) return path;

    for (string v = endId; v != startId; v = parent[v]) path.push_back(v);
    path.push_back(startId);
    reverse(path.begin(), path.end());
    return path;
}

vector<string> Graph::findPathByWeight(const string& startId, const string& endId) const {
    unordered_map<string, double> dist;
    unordered_map<string, string> parent;

    for (auto& [id, v] : vertices) {
        dist[id] = numeric_limits<double>::infinity();
    }
    dist[startId] = 0;

    using P = pair<double, string>;
    priority_queue<P, vector<P>, greater<P>> pq;
    pq.push({0, startId});

    while (!pq.empty()) {
        auto [d, u] = pq.top(); pq.pop();
        if (d > dist[u]) continue;

        for (const auto& e : edges) {
            string neighbor;
            double weight = e.getWeight();
            if (e.getFrom() == u) neighbor = e.getTo();
            else if (e.getTo() == u) neighbor = e.getFrom();
            else continue;

            double newDist = d + weight;
            if (newDist < dist[neighbor]) {
                dist[neighbor] = newDist;
                parent[neighbor] = u;
                pq.push({newDist, neighbor});
            }
        }
    }

    vector<string> path;
    if (dist[endId] == numeric_limits<double>::infinity()) return path;

    for (string v = endId; v != startId; v = parent[v]) path.push_back(v);
    path.push_back(startId);
    reverse(path.begin(), path.end());
    return path;
}

double Graph::distanceBetween(const std::string& fromId, const std::string& toId) const{
    vector<string> path = findPathByWeight(fromId, toId);
    if (path.empty()) return -1.0;

    double totalDist = 0.0;
    for (size_t i = 0; i + 1 < path.size(); ++i) {
        for (const auto& e : edges) {
            if ((e.getFrom() == path[i] && e.getTo() == path[i + 1]) ||
                (e.getTo() == path[i] && e.getFrom() == path[i + 1])) {
                totalDist += e.getWeight();
                break;
                }
        }
    }
    return totalDist;
}

bool Graph::checkConnectivity() const {
    if (vertices.empty()) return true;

    unordered_map<string, bool> visited;
    queue<string> q;

    string startId = vertices.begin()->first;
    q.push(startId);
    visited[startId] = true;

    while (!q.empty()) {
        string cur = q.front(); q.pop();
        for (const auto& e : edges) {
            string neighbor;
            if (e.getFrom() == cur) neighbor = e.getTo();
            else if (e.getTo() == cur) neighbor = e.getFrom();
            else continue;

            if (!visited[neighbor]) {
                visited[neighbor] = true;
                q.push(neighbor);
            }
        }
    }
    return visited.size() == vertices.size();
}

void Graph::printGraph() const {
    cout << "Graph structure:\n";
    for (const auto& e : edges) {
        cout << e.getFrom() << " --(" << e.getWeight() << ")--> " << e.getTo() << endl;
    }
}