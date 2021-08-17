#include <iostream>
#include <random>
#include <time.h>
#include "ProblemInstance.h"
#include "Solution.h"
#include "Reader.h"
#include "Construction.h"
#include "ReorderNodes.h"
#include "RemoveNodes.h"
#include "AddNodes.h"

//tamano vecindario
size_t BETA;

//cantidad de nodos a eliminar
int GAMMA;

// cantidad de nodos que se intentar insertar antes de recalentar
int EPSILON;

// el primer nodo se selecciona:
// dentro de los beta primeros -> corte=true
// dentro de t.odo el vecindario -> corte=false
bool CORTE = true;
float PD;

vector<unsigned int> SEEEDS = {1008, 1505, 494, 15, 937, 1801, 522, 7, 24, 116, 9574, 7121, 803, 347, 3795, 672, 49, 2647, 223, 236}; // {cut Neighborhood}

int main(int argc, char *argv[]) {
    if(not argv[2]){
        Reader r(to_string(3));
        r.readOutputs(SEEEDS, BETA, GAMMA, EPSILON, CORTE);
    }
    else{
        int instance(atoi(argv[1]));
        int seed(atoi(argv[2]));
        int BETA(atoi(argv[3]));
        int GAMMA(atoi(argv[4]));
        int EPSILON(atoi(argv[5]));
        float PD(atof(argv[6]));
        int it_int(atoi(argv[7]));
        int evaluaciones(atoi(argv[8]));
        
        int it_ext=evaluaciones/it_int;

        srand(seed);

        clock_t t0, t1, t2, timeToBest;

        t0 = clock();
        Reader r(to_string(instance));

        cout << "For instance " << instance << " seed " << seed << endl;
        ProblemInstance *problemInstance;

        problemInstance = r.readInputFile(500); // temperature

        auto *bestSolution = new Solution(problemInstance, seed);
        auto *lastSolution = new Solution(problemInstance, seed);

        auto *reorderNodes = new ReorderNodes(0); // 0->reordena todos, #n reordena n nodos por ruta
        auto *removeNodes = new RemoveNodes();
        auto *addNodes = new AddNodes();

        int better(0);
        int nobetter(0);
        int nofix(0);
        int fix(0);
        int newD(0);

        int aux(0);
        for (int i: problemInstance->qualities) {
            if (i == 0) {
                aux += 1;
            }
        }

        bool doBreak(false);

        for (int j = 0; j < it_ext; ++j) {
            //cout << "----------------------------->Reset " << j << endl;

            auto *currentSolution = new Solution(problemInstance, seed);

            if (newD == 5 or newD == 0) {
                if (aux == problemInstance->getNumberOfQualities()) {
                    currentSolution->changeDemands(PD);
                    newD = 1;
                }
            } else {
                currentSolution->unsatisfiedDemand = lastSolution->unsatisfiedDemand;
                currentSolution->newDemands = lastSolution->newDemands;
                newD += 1;
            }

            auto *construction = new Construction(BETA, currentSolution);
            construction->feasibleSolution(currentSolution, CORTE);
            //lastSolution->resetSolution(*currentSolution); //Comentada en el otro código

            if (construction->construct) {
                reorderNodes->movement(currentSolution);
                
                if (currentSolution->getTotalBenefit() > bestSolution->getTotalBenefit()) 
                   bestSolution->resetSolution(*currentSolution);

                lastSolution->resetSolution(*currentSolution);
            
                for (int i = 0; i < it_int; ++i) {
                    removeNodes->breakDemands(currentSolution, GAMMA);
                    addNodes->movement(currentSolution, EPSILON);

                    if (addNodes->fix) {
                        fix++;
                        reorderNodes->movement(currentSolution);
                        removeNodes->movement(currentSolution);
                        reorderNodes->movement(currentSolution);

                        if (currentSolution->getTotalBenefit() > bestSolution->getTotalBenefit()) {
                            bestSolution->resetSolution(*currentSolution);
	                    bestSolution->printSolution();

                            for (int k: currentSolution->unsatisfiedDemand){
                                if(k>0){
                                    doBreak = true;
                                    break;
                                }
                            }
                            t2 = clock();
                            timeToBest = ((double) (t2 - t0)) / CLOCKS_PER_SEC;
                            better++;
                        } else {
                            nobetter++;
                        }
                        lastSolution->resetSolution(*currentSolution);
                    } else {
                        nofix++;
                        currentSolution->resetSolution(*lastSolution);
                    }
                    currentSolution->temperature = currentSolution->temperature * 0.99;
                }
            }
            delete currentSolution;
            if (doBreak){
                break;
            }
        }

        for (Route *route:bestSolution->routes) {
            route->resetRouteType();
        }

	bestSolution->printSolution();
        cout << timeToBest<<endl;

        delete removeNodes;
        delete reorderNodes;
        delete lastSolution;
        delete bestSolution;
        delete problemInstance;

    }

    return 0;
}
