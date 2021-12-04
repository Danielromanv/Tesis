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
    int runs(atoi(argv[3]));
    int steps(atoi(argv[4]));
    int castigo(atoi(argv[5]));
    int n =0;
    float slack = 0.0;
    srand(seed);

    clock_t tStart = clock();

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
    Solution *rsolucion = new Solution(problemInstance, seed);
    Solution *Msolucion = new Solution(problemInstance, seed);
     // Revisar si dejar dentro o fuera
    Movement *move = new Movement();

    while (n < runs) {
        Construction *construct = new Construction(0, solucion);
        construct -> feasibleSolution(solucion, slack);
        solucion->isFeasible();


        if(n == 0){
            Msolucion->resetSolution(*solucion);
            std::cout << Msolucion->PunishEvaluate(castigo)[0] << std::endl;
        }


        //comparar si es mejor que la mejorsolucion y reemplazar y reset solution de la que varia
        if (solucion->PunishEvaluate(castigo)[0] > Msolucion->PunishEvaluate(castigo)[0]){
            Msolucion->resetSolution(*solucion);
        }

        double ra,rb;

        //tomo la solucion, veo si cambiar con 2opt, le doy un i un j de posicion de la Ruta
        //va devolver la diferencia de distancia

        //While haciendo exchanges entre rutas con parametro de steps
        int m = 0, stuck = 0, pert = 1;
        double prev;
        while(m < steps){
            for (size_t i = 0; i < solucion->routes.size(); i++) {
                do {
                    ra = solucion->routes[i]->distance;
                    move->ToptNeigborhood(solucion, solucion->routes[i]);
                    solucion->routes[i]->DetectWrong();
                    rb = solucion->routes[i]->distance;
                }while(rb < ra);
            }
            do{
                prev = solucion->PunishEvaluate(castigo)[0];
                for (size_t i = 0; i < solucion->routes.size()-1; i++) {
                    //Elegir la Ruta A
                    for (size_t j = i+1; j < solucion->routes.size(); j++) {
                        //Elegir la ruta B
                        move->ExNeiborhood(solucion, solucion->routes[i], solucion->routes[j],castigo);
                    }
                }
            }while(solucion->PunishEvaluate(castigo)[0] > prev);
            if (solucion->PunishEvaluate(castigo)[0] > Msolucion->PunishEvaluate(castigo)[0]){
                Msolucion->resetSolution(*solucion);
                std::cout << Msolucion->PunishEvaluate(castigo)[0] <<" "<<  Msolucion->PunishEvaluate(castigo)[1]<< " "<<  Msolucion->PunishEvaluate(castigo)[2]<< " "<<  Msolucion->PunishEvaluate(castigo)[3] << " "<< problemInstance->qualities[0]-Msolucion->recollected[0]<< " "<< problemInstance->qualities[1]-Msolucion->recollected[1]<< " "<< problemInstance->qualities[2]-Msolucion->recollected[2] << std::endl;
                stuck = 0;
            }
            stuck++;
            if (stuck > 0.1*steps) {
                pert++;
                stuck = 0;
                if (pert>5){
                    pert = 1;
                }
            }
            move->AddCandidates(solucion,pert);
            m++;
        }



        if (solucion->PunishEvaluate(castigo)[0] > Msolucion->PunishEvaluate(castigo)[0]){
            Msolucion->resetSolution(*solucion);
            std::cout << Msolucion->PunishEvaluate(castigo)[0] << std::endl;
        }

        solucion->resetSolution(*rsolucion);
        //comparar si es mejor que la mejorsolucion y reemplazar y reset solution de la que varia

        std::cout << "n: "<< n << '\n';
        delete construct;
        n++;
        slack = (float)n/(float)runs;
    }
    Msolucion->printAll();
    std::cout << Msolucion->PunishEvaluate(castigo)[0]<< " "<< (double)(clock() - tStart)/CLOCKS_PER_SEC<<" "<<  Msolucion->PunishEvaluate(castigo)[1]<< " "<<  Msolucion->PunishEvaluate(castigo)[2]<< " "<<  Msolucion->PunishEvaluate(castigo)[3] << " "<< problemInstance->qualities[0]-Msolucion->recollected[0]<< " "<< problemInstance->qualities[1]-Msolucion->recollected[1]<< " "<< problemInstance->qualities[2]-Msolucion->recollected[2] << std::endl;

    delete problemInstance;
    delete move;
    delete solucion;
    delete rsolucion;
    delete Msolucion;

return 0;
}
