#pragma once
#include <iostream>
#include <vector>
#include <random>
#include "Trip.h"
#include "ProblemInstance.h"
#include "Route.h"
#include "Solution.h"
#include <cmath>

using namespace std;

class Movement {
public:
    double bestDistance;
    int bestIndex;
    Node *node;


public:
    Movement();
    ~Movement();

    double ToptCheck(Solution *solution, int a, int b, Route * routes);
    Route * createNewRoute(int desde, int hasta, Route *route, Solution * solution);
    void neigborhood(Solution *solution, Route * route);
};
