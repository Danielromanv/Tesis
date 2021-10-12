#pragma once
#include <iostream>
#include <vector>
#include <random>
#include "Trip.h"
#include "ProblemInstance.h"
#include "Route.h"
#include "Solution.h"

using namespace std;

class Construction{
public:
    int solid;
    int totalProduction;
    vector<Trip *> neighborhood;

    int currentType;
    Node *currentNode;
    Route *currentRoute;
    bool construct;

public:
    explicit Construction(int solid, Solution *solution);
    ~Construction();

    void feasibleSolution(Solution *solution);
};
