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
    int n =0, c = 0;
    float slack = 0.0;
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
    Solution *rsolucion = new Solution(problemInstance, seed);
    Solution *Msolucion = new Solution(problemInstance, seed);
     // Revisar si dejar dentro o fuera
    Movement *move = new Movement();

    while (n < runs) {
        Construction *construct = new Construction(0, solucion);
        construct -> feasibleSolution(solucion, slack);
        solucion->isFeasible();
        // std::cout << "SOLUCION POST CONSTRUCCION" << '\n';
        // solucion->printAll();

        if(n == 0){
            Msolucion->resetSolution(*solucion);
        }


        //comparar si es mejor que la mejorsolucion y reemplazar y reset solution de la que varia
        //std::cout << "/* message */"<< Msolucion->PunishEvaluate(castigo) << '\n';
        if (solucion->PunishEvaluate(castigo) > Msolucion->PunishEvaluate(castigo)){
            Msolucion->resetSolution(*solucion);
        }
        //std::cout << "Msolucion reset1" << '\n';
        //Msolucion->printAll();


        //tomo la solucion, veo si cambiar con 2opt, le doy un i un j de posicion de la Ruta
        //va devolver la diferencia de distancia
        //std::cout << "PREVIO A 2OPT" << '\n';

        //solucion->printAll();

        double a = solucion->getDistance(), obj = solucion->PunishEvaluate(castigo),ra,rb;
        for (size_t i = 0; i < solucion->routes.size(); i++) {
            do {
                ra = solucion->routes[i]->distance;
                move->neigborhood(solucion, solucion->routes[i]);
                solucion->routes[i]->DetectWrong();
                rb = solucion->routes[i]->distance;
            }while(rb < ra);
        }

        //std::cout << "Posterior a 2OPT" << '\n';

        //solucion->printAll();

        //double b = solucion->getDistance(), nobj = solucion->PunishEvaluate(castigo);
        //std::cout << "distancia original: "<< a << '\n'<< "nueva distancia: "<< b<<"\nmejora: "<< abs(a-b) <<'\n';
        //std::cout << "Fobj original: "<< obj << '\n'<< "nueva Fobj: "<< nobj <<"\nmejora: "<< abs(obj-nobj) <<'\n';
        if (solucion->PunishEvaluate(castigo) > Msolucion->PunishEvaluate(castigo)){
            // std::cout << "actual = "<< solucion->PunishEvaluate(castigo) << " best = "<< Msolucion->PunishEvaluate(castigo) ;
            // std::cout << "slack = "<< slack << '\n';
            Msolucion->resetSolution(*solucion);
        }
        //std::cout << "n: "<< n << '\n';
        //std::cout << "MEJOR SOL post RESET" << '\n';
        //Msolucion->printAll();
        //std::cout << "DetectWrong"<< solucion->DetectWrong() << '\n';
        solucion->resetSolution(*rsolucion);
        //comparar si es mejor que la mejorsolucion y reemplazar y reset solution de la que varia

        //double fo = solucion->evaluate();
        //std::cout << "solucion POST RESET" << '\n';
        //solucion->printAll();

        //std::cout << "F.O: "<< fo << '\n';
        delete construct;
        n++;
        slack = (float)n/(float)runs;
    }
    //std::cout << " best = "<< Msolucion->PunishEvaluate(castigo) << '\n';
    std::cout << Msolucion->PunishEvaluate(castigo) << '\n';
    //Msolucion->printAll();
    delete problemInstance;
    delete move;
    delete solucion;
    delete rsolucion;
    delete Msolucion;

return 0;
}
