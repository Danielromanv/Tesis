#include <iostream>
#include <random>
#include <time.h>
#include "ProblemInstance.h"
#include "Reader.h"
#include "Solution.h"
#include "Construction.h"
#include "Movement.h"
float MilkWeight[] = {30,21,9};


int main(int argc, char *argv[]) {
    int instance(atoi(argv[1]));
    int seed(atoi(argv[2]));
    int runs(atoi(argv[3]));
    int steps(atoi(argv[4]));
    int castigo(atoi(argv[5]));
    int n = 0;
    float slack = 1.0;

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
    bool debug = false;
    while (n < runs) {
        slack = solucion->random_number(0.7, 1);
        Construction *construct = new Construction(0, solucion);
        construct->feasibleSolution(solucion, slack);

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
        int m = 0, stuck = 0, pert = 2, intentos, max_intentos, s;
        double prev;
        while(m < steps){
            /////////////////////////////// 2-opt ///////////////////////////////////////////////////////////
            for (size_t i = 0; i < solucion->routes.size(); i++) {
                do {
                    ra = solucion->routes[i]->distance;
                    move->ToptNeigborhood(solucion, solucion->routes[i]);
                    solucion->routes[i]->DetectWrong();
                    rb = solucion->routes[i]->distance;
                    if (solucion->PunishEvaluate(castigo)[0] > Msolucion->PunishEvaluate(castigo)[0]){
                        Msolucion->resetSolution(*solucion);
                        std::cout << "n: " << n << " m:" << m << " slack: " << slack << " 2opt----->" << Msolucion->PunishEvaluate(castigo)[0] <<" "<<  Msolucion->PunishEvaluate(castigo)[1]<< " "<<  Msolucion->PunishEvaluate(castigo)[2]<< " "<<  Msolucion->PunishEvaluate(castigo)[3] << " "<< problemInstance->qualities[0]-Msolucion->recollected[0]<< " "<< problemInstance->qualities[1]-Msolucion->recollected[1]<< " "<< problemInstance->qualities[2]-Msolucion->recollected[2] << std::endl;
                        if (debug) solucion->printShort();
                        if (debug) getchar();
                    }
                    
                }while(rb < ra);
            }
            
            
            /////////////////////////////// Exchange ///////////////////////////////////////////////////////////
            do{
                prev = solucion->PunishEvaluate(castigo)[0];
                for (size_t i = 0; i < solucion->routes.size()-1; i++) {
                    //Elegir la Ruta A
                    for (size_t j = i+1; j < solucion->routes.size(); j++) {
                        //Elegir la ruta B
                        move->ExNeiborhood(solucion, solucion->routes[i], solucion->routes[j],castigo);
                        if (solucion->PunishEvaluate(castigo)[0] > Msolucion->PunishEvaluate(castigo)[0]){
                        
                            Msolucion->resetSolution(*solucion);
                            std::cout << "n: " << n << " m:" << m << " slack: " << slack << "  Exch-----> " << Msolucion->PunishEvaluate(castigo)[0] <<" "<<  Msolucion->PunishEvaluate(castigo)[1]<< " "<<  Msolucion->PunishEvaluate(castigo)[2]<< " "<<  Msolucion->PunishEvaluate(castigo)[3] << " "<< problemInstance->qualities[0]-Msolucion->recollected[0]<< " "<< problemInstance->qualities[1]-Msolucion->recollected[1]<< " "<< problemInstance->qualities[2]-Msolucion->recollected[2] << std::endl;
                            if (debug) solucion->printShort();
                            if (debug) getchar();
                        }   
                    }
                }
            }while(solucion->PunishEvaluate(castigo)[0] > prev);
            
            /////////////////////////////// Limpieza ///////////////////////////////////////////////////////////
            max_intentos = 2 * solucion->routes.size();
            intentos = 0;
            while (intentos < max_intentos) {
                s = solucion->random_int_number(0,solucion->routes.size()-1);
                if(move->checkRoute(solucion, solucion->routes[s])[0] == 1){
                    move->purify(solucion, solucion->routes[s]);
                    intentos = max_intentos;
                }
                intentos++;
            }
            
            if (debug) cout << "Post Limpieza" << endl;
            if (debug) solucion->printShort();
            if (debug) getchar();

            
            if (solucion->PunishEvaluate(castigo)[0] > Msolucion->PunishEvaluate(castigo)[0]){
                Msolucion->resetSolution(*solucion);
                std::cout << "n: " << n << " m:" << m << " slack: " << slack << "   Limpieza -->" << Msolucion->PunishEvaluate(castigo)[0] <<" "<<  Msolucion->PunishEvaluate(castigo)[1]<< " "<<  Msolucion->PunishEvaluate(castigo)[2]<< " "<<  Msolucion->PunishEvaluate(castigo)[3] << " "<< problemInstance->qualities[0]-Msolucion->recollected[0]<< " "<< problemInstance->qualities[1]-Msolucion->recollected[1]<< " "<< problemInstance->qualities[2]-Msolucion->recollected[2] << std::endl;
                if (debug) solucion->printShort();
                if (debug) getchar();
            }   

            /////////////////////////////// Limpieza ///////////////////////////////////////////////////////////
            move->AddCandidates(solucion, pert);
            if (debug) cout << "Post Agregado" << endl;
            if (debug) solucion->printShort();
            if (debug) getchar();
            
            if (solucion->PunishEvaluate(castigo)[0] > Msolucion->PunishEvaluate(castigo)[0]){
                        
                Msolucion->resetSolution(*solucion);
                std::cout << "n: " << n << " m:" << m << " slack: " << slack <<  "    AddCand-->" << Msolucion->PunishEvaluate(castigo)[0] <<" "<<  Msolucion->PunishEvaluate(castigo)[1]<< " "<<  Msolucion->PunishEvaluate(castigo)[2]<< " "<<  Msolucion->PunishEvaluate(castigo)[3] << " "<< problemInstance->qualities[0]-Msolucion->recollected[0]<< " "<< problemInstance->qualities[1]-Msolucion->recollected[1]<< " "<< problemInstance->qualities[2]-Msolucion->recollected[2] << std::endl;
                if (debug) solucion->printShort();
                if (debug) getchar();
            }   

            m++;
        }

        if (solucion->PunishEvaluate(castigo)[0] > Msolucion->PunishEvaluate(castigo)[0]){
            Msolucion->resetSolution(*solucion);
            std::cout << Msolucion->PunishEvaluate(castigo)[0] <<" "<<  Msolucion->PunishEvaluate(castigo)[1]<< " "<<  Msolucion->PunishEvaluate(castigo)[2]<< " "<<  Msolucion->PunishEvaluate(castigo)[3] << " "<< problemInstance->qualities[0]-Msolucion->recollected[0]<< " "<< problemInstance->qualities[1]-Msolucion->recollected[1]<< " "<< problemInstance->qualities[2]-Msolucion->recollected[2] << std::endl;

        }

        solucion->resetSolution(*rsolucion);
        //comparar si es mejor que la mejorsolucion y reemplazar y reset solution de la que varia

        delete construct;
        n++;
        //slack = 1.00 - (float)n/(float)runs;
    }
    Msolucion->printAll();
    std::cout << Msolucion->PunishEvaluate(castigo)[0]<< " "<< (double)(clock() - tStart)/CLOCKS_PER_SEC<<" "<<  Msolucion->PunishEvaluate(castigo)[1]<< " "<<  Msolucion->PunishEvaluate(castigo)[2]<< " "<<  Msolucion->PunishEvaluate(castigo)[3] << " "<< problemInstance->qualities[0]-Msolucion->recollected[0]<< " "<< problemInstance->qualities[1]-Msolucion->recollected[1]<< " "<< problemInstance->qualities[2]-Msolucion->recollected[2] << std::endl;
    //vector<double> p= move->checkRoute(Msolucion, Msolucion->routes[5]);
    //std::cout << p[0]<< " "<<p[1]<<" "<<p[2]<< " " <<p[3] << '\n';
    //Msolucion->newRecollected();
    // move->purify(Msolucion, Msolucion->routes[5]);
    // Msolucion->routes[5]->printAll();
    // std::cout << Msolucion->PunishEvaluate(castigo)[0]<< "\n";
    delete problemInstance;
    delete move;
    delete solucion;
    delete rsolucion;
    delete Msolucion;

return 0;
}
