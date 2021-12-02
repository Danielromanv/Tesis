#pragma once
#include <iostream>
#include <vector>
#include <random>
#include <stdio.h>
#include <algorithm>
#include <cstring>
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
    void ToptNeigborhood(Solution *solution, Route * route);
    double ExEvaluate(Solution *solution,double punish, Route * routeA, Route * routeB, double Sa, double Sb, double Da, double Db, int Ta, int Tb);
    double * ExCheck(Solution *solution, int a, int b, int la, int lb, Route * routeA, Route * routeB, double punish);
    void ChangeTrip(Solution *solution, int a, int b, int la, int lb, Route * routeA, Route * routeB, double punish);
    Route * ChangeRTrip(Solution *solution, int a, int b, int la, int lb, Route * routeA, Route * routeB, double punish);
    void ExNeiborhood(Solution *solution, Route * routeA, Route * routeB, double punish);
    vector<Node *> getCandidates(Solution * solution, int a);
    void RemoveFromRoute(Solution * solution, int a);
    void AddCandidates(Solution * solution);

};
