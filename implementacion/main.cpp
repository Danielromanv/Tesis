#include <iostream>
#include <random>
#include <time.h>
#include "ProblemInstance.h"
#include "Reader.h"
#include "Solution.h"
#include "Construction.h"
#include "Movement.h"

int main(int argc, char *argv[]) {
    int instance(atoi(argv[1]));
    int seed(atoi(argv[2]));
    float slack = 0.2;
    float castigo = 1000;
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
    solucion->PercentageLeft();
    solucion->isFeasible();
    solucion->PunishEvaluate(castigo);

    //tomo la solucion, veo si cambiar con 2opt, le doy un i un j de posicion de la Ruta
    //va devolver la diferencia de distancia
    Movement *move = new Movement();
    move->ToptCheck(solucion,1,6,solucion->routes[0]);
    solucion->routes[0]->printAll();
    Route *a = move->createNewRoute(0,7,solucion->routes[0],solucion);
    a->printAll();
    //double fo = solucion->evaluate();
    //solucion->printAll();
    //std::cout << "F.O: "<< fo << '\n';

    delete problemInstance;
return 0;
}
