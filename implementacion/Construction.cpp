#include "Construction.h"
#include "algorithm"

Construction::Construction(int solid, Solution *solution):solid(solid),totalProduction(0), currentType(1), currentNode(solution->plant), currentRoute(solution->routes.back()), construct(true){};

Construction::~Construction(){
    cout<< "Deleting construction" << endl;

    for (Trip *t:this->neighborhood) {
        delete t;
    }
    this->neighborhood.clear();
    this->neighborhood.shrink_to_fit();

    this->currentNode = nullptr;
    this->currentRoute = nullptr;
}

void Construction::feasibleSolution(Solution *solution, float slack){
    for(int i = 0; i < solution->problemInstance->getNumberOfQualities(); ++i){
        std::cout << "Nodos iteracion tipo "<< i << '\n';
        for (Node *n: solution->problemInstance->NodesByType[i]){n->printAll();}
        while (solution->getUnsatisfiedType(i) != -1) {
            //std::cout << "unsatisfiedDemand "<< i << " : "<< solution->unsatisfiedDemand[i] << '\n';
            vector<Trip *> opts;
            for (Node *n: solution->problemInstance->NodesByType[i]){
                //std::cout << this->currentRoute->fitsInTruck(n) << '\n';
                if (this->currentRoute->fitsInTruck(n) && find(solution->unvisitedNodes.begin(),solution->unvisitedNodes.end(),n) != solution->unvisitedNodes.end()){
                    //n->printAll();
                    opts.push_back(solution->newTrip(this->currentNode, n, this->currentRoute));
                }
            }
            if(opts.empty() && !this->currentRoute->isFull()){
                this->currentRoute->setFull();
                std::cout << "vuelve a planta" << "\n\n\n";
                opts.push_back(solution->newTrip(this->currentNode, solution->plant, this->currentRoute));
            }
            if (opts.size() > 0){
                sort(opts.begin(), opts.end(), sortByDistance);
                // for (Trip *t: opts){
                //     t->printAll();
                // }
                opts.front() -> printAll();
                solution->addTrip(opts.front(),this->currentRoute);
                solution->stepUpdateSolution(opts.front(), this->currentRoute, false);
                this->currentNode = opts.front()->finalNode;
                //std::cout << "remaining: "<< this->currentRoute->remainingCapacity << '\n';
                if (this->currentRoute->isFull()){
                    this->currentRoute->printAll();
                    for (int i=0; i<3;i++){
                        std::cout << "recolectado de "<<i<<" : "<< solution-> recollected[i] << '\n';
                    }
                    if ((solution->unusedTrucks.empty() && !solution->routes.back()->trips.empty() &&
                        solution->routes.back()->trips.back()->finalNode == solution->plant) || (solution->unsatisfiedDemand[i] <= 0)){
                        std::cout << "cambio de tipo" << '\n';
                        solution->addRoute(i+1);
                        this->currentRoute = solution->routes.back();
                        this->currentNode = solution->plant;
                        this->currentRoute->printAll();
                        break;
                    }
                    // if (checkUsage(slack,i,solution) && i < 2){
                    //     solution->addRoute(i+2);
                    //     this->currentRoute = solution->routes.back();
                    //     this->currentNode = solution->plant;
                    //     this->currentRoute->printAll();
                    //     break;
                    // }
                    solution->addRoute(i+1);
                    this->currentRoute = solution->routes.back();
                    this->currentNode = solution->plant;
                    this->currentRoute->printAll();
                    if (checkUsage(slack,i,solution) && i < 2){
                        this->currentRoute->type = i+1;
                        std::cout << "cambio forzado" << '\n';
                        break;
                    }
                }
            }
        }

    }
}

bool Construction::checkUsage(float slack, int index, Solution *solution){
    int available = 0;
    for (Node *n: solution->problemInstance->NodesByType[index]){
        if(find(solution->unvisitedNodes.begin(),solution->unvisitedNodes.end(),n) != solution->unvisitedNodes.end()){
            available += n->getProduction();
        }
    }
    cout << "\n\ndisponible de "<< index<< " para recolectar: "<< available << '\n';
    cout << "disponible en el camión: "<< this->currentRoute->remainingCapacity << '\n';
    cout << "disponible en el camión con slack: "<< (this->currentRoute->remainingCapacity)*(1-slack) << "\n\n";
    return ((this->currentRoute->remainingCapacity)*(1-slack) > available);
}
