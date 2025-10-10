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
#include "../Logger/LoggerGlobal.h"

using namespace std;

//===========Vertex===========
Vertex::Vertex() {
    logger.debug("Creating new Vertex...");
    UUID uuid;
    RPC_STATUS status = UuidCreate(&uuid);
    if (status != RPC_S_OK && status != RPC_S_UUID_LOCAL_ONLY && status != RPC_S_UUID_NO_ADDRESS) {
        logger.error("UuidCreate failed, RPC_STATUS = " + to_string(status));
        throw runtime_error("UuidCreate failed, RPC_STATUS = " + to_string(status));
    }

    RPC_CSTR strUuid = nullptr;
    status = UuidToStringA(&uuid, &strUuid);
    if (status != RPC_S_OK) {
        logger.error("UuidToString failed, RPC_STATUS = " + to_string(status));
        throw runtime_error("UuidToString failed, RPC_STATUS = " + to_string(status));
    }

    id = string(reinterpret_cast<char*>(strUuid));
    RpcStringFreeA(&strUuid);

    logger.debug("Vertex created with id = " + id);
}
Vertex::Vertex(const string& id) : id(move(id)) {}

Vertex::~Vertex() {
    logger.debug("Vertex destroyed: " + id);
}

string Vertex::getId() const {
    logger.debug("getId() called for Vertex: " + id);
    return id;
}

//===========Edge===========
string Edge::getFrom() const { return fromId; }
string Edge::getTo() const { return toId; }
double Edge::getWeight() const { return weight; }

//===========Graph===========

const unordered_map<string, shared_ptr<Vertex>>& Graph::getVertices() const {
    logger.debug("getVertices() called");
    return vertices;
}

vector<string> Graph::getNeighbors(const string& id) const {
    logger.debug("getNeighbors() for id = " + id);
    vector<string> neighbors;

    for (const auto& e : edges) {
        if (e.getFrom() == id) {
            neighbors.push_back(e.getTo());
        } else if (e.getTo() == id) {
            neighbors.push_back(e.getFrom());
        }
    }

    logger.debug("Found " + to_string(neighbors.size()) + " neighbors for vertex " + id);
    return neighbors;
}

void Graph::setVertices(const unordered_map<string, shared_ptr<Vertex>>& newVertices) {
    logger.debug("Setting new vertices list, count = " + to_string(newVertices.size()));
    vertices = newVertices;
}

void Graph::addVertex(shared_ptr<Vertex> v) {
    logger.debug("Adding vertex with id = " + v->getId());
    vertices[v->getId()] = v;
}

shared_ptr<Vertex> Graph::getVertex(const string& id) const {
    logger.debug("getVertex() called for id = " + id);
    auto it = vertices.find(id);
    if (it != vertices.end()) {
        logger.debug("Vertex found: " + id);
        return it->second;
    }
    logger.warn("Vertex not found: " + id);
    return nullptr;
}

const Edge* Graph::getEdge(const string& fromId, const string& toId) const {
    logger.debug("getEdge() between " + fromId + " and " + toId);
    for (const auto& e : edges) {
        if ((e.getFrom() == fromId && e.getTo() == toId) ||
            (e.getFrom() == toId && e.getTo() == fromId)) {
            logger.debug("Edge found between " + fromId + " and " + toId);
            return &e;
        }
    }
    logger.warn("Edge not found between " + fromId + " and " + toId);
    return nullptr;
}

void Graph::removeVertex(const string& id) {
    logger.debug("Removing vertex id = " + id);
    if (!vertices.count(id)) {
        logger.warn("Vertex with id " + id + " not found");
        return;
    }

    vertices.erase(id);
    edges.erase(remove_if(edges.begin(), edges.end(),
                          [&](const Edge& e) {
                              return e.getFrom() == id || e.getTo() == id;
                          }),
                edges.end());
    logger.debug("Vertex " + id + " and its edges removed");
}

const vector<Edge>& Graph::getEdges() const {
    logger.debug("getEdges() called, count = " + to_string(edges.size()));
    return edges;
}

void Graph::setEdges(const vector<Edge>& newEdges) {
    logger.debug("Setting new edge list, count = " + to_string(newEdges.size()));
    edges = newEdges;
}

void Graph::addEdge(const string& fromId, const string& toId, double weight) {
    logger.debug("Adding edge: " + fromId + " <-> " + toId + " weight=" + to_string(weight));
    if (vertices.count(fromId) && vertices.count(toId)) {
        edges.emplace_back(fromId, toId, weight);
        edges.emplace_back(toId, fromId, weight);
        logger.debug("Edge successfully added");
    } else {
        logger.warn("Cannot add edge: one or both vertices not found (" + fromId + ", " + toId + ")");
    }
}

void Graph::removeEdge(const string& fromId, const string& toId) {
    logger.debug("Removing edge between " + fromId + " and " + toId);
    size_t before = edges.size();

    edges.erase(remove_if(edges.begin(), edges.end(),
                          [&](const Edge& e) {
                              return (e.getFrom() == fromId && e.getTo() == toId) ||
                                     (e.getFrom() == toId && e.getTo() == fromId);
                          }),
                edges.end());

    logger.debug("Edges removed: " + to_string(before - edges.size()));
}

vector<string> Graph::findPath(const string& startId, const string& endId) {
    logger.debug("Finding path (unweighted) from " + startId + " to " + endId);

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
    if (!visited[endId]) {
        logger.warn("Path not found from " + startId + " to " + endId);
        return path;
    }

    for (string v = endId; v != startId; v = parent[v]) path.push_back(v);
    path.push_back(startId);
    reverse(path.begin(), path.end());

    logger.debug("Path found: length = " + to_string(path.size()));
    return path;
}

vector<string> Graph::findPathByWeight(const string& startId, const string& endId) const {
    logger.debug("Finding shortest path (by weight) from " + startId + " to " + endId);

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
    if (dist[endId] == numeric_limits<double>::infinity()) {
        logger.warn("No weighted path found between " + startId + " and " + endId);
        return path;
    }

    for (string v = endId; v != startId; v = parent[v]) path.push_back(v);
    path.push_back(startId);
    reverse(path.begin(), path.end());

    logger.info("Shortest weighted path found: total weight = " + to_string(dist[endId]));
    return path;
}

double Graph::distanceBetween(const string& fromId, const string& toId) const {
    logger.debug("Calculating distanceBetween() " + fromId + " <-> " + toId);

    vector<string> path = findPathByWeight(fromId, toId);
    if (path.empty()) {
        logger.warn("No path between " + fromId + " and " + toId);
        return -1.0;
    }

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

    logger.info("Total distance between " + fromId + " and " + toId + " = " + to_string(totalDist));
    return totalDist;
}

bool Graph::checkConnectivity() const {
    logger.debug("Checking connectivity of graph");

    if (vertices.empty()) {
        logger.warn("Graph is empty, considered connected by default");
        return true;
    }

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

    bool connected = (visited.size() == vertices.size());
    logger.debug(string("Graph connectivity check result: ") + (connected ? "Connected" : "Disconnected"));
    return connected;
}

void Graph::printGraph() const {
    logger.debug("Printing graph structure");
    cout << "Graph structure:\n";
    for (const auto& e : edges) {
        cout << e.getFrom() << " --(" << e.getWeight() << ")--> " << e.getTo() << endl;
    }
}
