#pragma once
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cmath>
#include <list>
#include <vector>
#include "ProblemInstance.h"

using namespace std;

class Reader
{
private:
    string filePath;
    ifstream input;
    string plant;

public:
    explicit Reader(string option);
    ~Reader();

    ProblemInstance *readInputFile();
    //void readOutputs(vector<unsigned int> seeds, size_t beta, int gamma, int epsilon, bool corte);

private:
    void findDef(string def);
    ProblemInstance *createInstance();

    void readTrucks(ProblemInstance *problemInstance);
    void readQuality(ProblemInstance *problemInstance);
    void readNodes(ProblemInstance *problemInstance);
    void readDistances(ProblemInstance *problemInstance);
};
