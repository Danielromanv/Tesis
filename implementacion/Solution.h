#pragma once
#include <iostream>
#include <fstream>
#include <random>
#include <numeric>
#include <vector>
#include "Truck.h"
#include "Node.h"
#include "ProblemInstance.h"
#include "Route.h"

using namespace std;

class Solution {
public:
    vector<int> recollected; //x type nodo
    vector<int> unsatisfiedDemand;

    vector<Truck *> unusedTrucks;
    vector<Node *> unvisitedNodes;

    vector<int> nodesXQuality;

    vector<Route *> routes;

    ProblemInstance *problemInstance;

    Node * plant;
    vector<double> literCost;
    double kilometerCost;
    unsigned int seed;


public:
    explicit Solution(ProblemInstance *problemInstance, unsigned int seed);
    ~Solution();
    Solution(const Solution &s2);

    double random_number(double min, double max);
    int random_int_number(int min, int max);

    void setQualities(double size);

    void addTruck(Truck *truck);
    void addNode(Node *node);
    void addRoute(int type);
    void addTrip(Trip *trip, Route *route);
    void stepUpdateSolution(Trip *trip, Route *route, bool repairing);
    void updateDemands(int currentTypeIndex, Node *currentNode, Route *currentRoute, int production, bool repairing);
    void addBackToPlant(int i, Node *currentNode, Route *currentRoute, bool pos, bool repairing);

    void insertTrip(Route *route, int index, Node *node);
    void insertNode(Node *node);
    void removeTruck(Truck *truck);
    void removeNode(Node *node);
    Trip *newTrip(Node *node1, Node *node2, Route *route);

    int getDemandSubtraction(int dda, int prod);
    Node *getCurrentNode();
    Truck *getNextTruck();
    int getUnsatisfiedType(int from);
    char getType(int i, bool reverse);

    void printAll();
    void printSolution();

    bool DetectWrong(void);


};
