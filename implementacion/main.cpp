#include <iostream>
#include <random>
#include <time.h>
#include "ProblemInstance.h"
#include "Reader.h"
#include "Solution.h"
#include "Construction.h"

int main(int argc, char *argv[]) {
    int instance(atoi(argv[1]));
    int seed(atoi(argv[2]));
    float slack = 0.10;
    srand(seed);

    clock_t t0, t1, t2, timeToBest;

    t0 = clock();
    Reader r(to_string(instance));

    cout << "For instance " << instance << " seed " << seed << endl;
    ProblemInstance *problemInstance;
    problemInstance = r.readInputFile();
    problemInstance -> printAll();

    int aux(0);
    for (int i: problemInstance->qualities) {
        if (i == 0) {
            aux += 1;
        }
    }
    //crear objeto solucion
    //probar el constructor del Objetivo
    //muestra la solucion
    //evaluar lo que me muestre

    Solution *solucion = new Solution(problemInstance, seed);
    Construction *construct = new Construction(0, solucion);
    construct -> feasibleSolution(solucion, slack);
    //solucion -> printAll();
    vector<int> l = solucion->left();
    solucion->isFeasible();
    //double fo = solucion->evaluate();
    //solucion->printAll();
    //std::cout << "F.O: "<< fo << '\n';

    delete problemInstance;
return 0;
}
