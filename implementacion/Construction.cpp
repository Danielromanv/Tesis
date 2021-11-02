#include "Construction.h"
#include "algorithm"

Construction::Construction(int solid, Solution *solution):solid(solid),totalProduction(0), currentType(1), currentNode(solution->plant), currentRoute(solution->routes.back()), construct(true){};

Construction::~Construction(){
    //cout<< "Deleting construction" << endl;

    for (Trip *t:this->neighborhood) {
        delete t;
    }
    this->neighborhood.clear();
    this->neighborhood.shrink_to_fit();

    this->currentNode = nullptr;
    this->currentRoute = nullptr;
}

void Construction::feasibleSolution(Solution *solution, float slack){
    vector<int> real = getRealDemand(solution);
    for(int i = 0; i < solution->problemInstance->getNumberOfQualities(); ++i){
        // std::cout << "Nodos iteracion tipo "<< i << '\n';
        if (this->currentRoute->isFull()){
            break;
        }
        //for (Node *n: solution->problemInstance->NodesByType[i]){n->printAll();}
        while (solution->getUnsatisfiedType(i) != -1 || this->currentRoute->remainingCapacity > 0 || solution->recollected[i] < real[i] ) {
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
                // std::cout << "vuelve a planta" << "\n\n\n";
                opts.push_back(solution->newTrip(this->currentNode, solution->plant, this->currentRoute));
            }
            if (opts.size() > 0){
                sort(opts.begin(), opts.end(), sortByDistance);

                if (this->currentRoute->distance == 0){
                    int selected = rand() % opts.size();
                    // opts[selected]->printAll();
                    solution->addTrip(opts[selected],this->currentRoute);
                    solution->stepUpdateSolution(opts[selected], this->currentRoute, false);
                    this->currentNode = opts[selected]->finalNode;
                }
                else{
                    // opts.front() -> printAll();
                    solution->addTrip(opts.front(),this->currentRoute);
                    solution->stepUpdateSolution(opts.front(), this->currentRoute, false);
                    this->currentNode = opts.front()->finalNode;
                }
                //std::cout << "remaining: "<< this->currentRoute->remainingCapacity << '\n';
                if (this->currentRoute->isFull()){
                    // this->currentRoute->printAll();
                    for (int i=0; i<3;i++){
                        // std::cout << "recolectado de "<<i<<" : "<< solution-> recollected[i] << '\n';
                    }
                    if ((solution->unusedTrucks.empty() && !solution->routes.back()->trips.empty() &&
                        solution->routes.back()->trips.back()->finalNode == solution->plant) || (solution->unsatisfiedDemand[i] <= 0  && solution->problemInstance->qualities[i] > 0)){
                        // std::cout << "cambio de tipo" << '\n';
                        solution->addRoute(i+1);
                        // if (this->currentNode->getId() == solution->plant->getId()){
                        //     checkUsage(slack,i,solution);
                        //     std::cout << "acá es" << '\n';
                        //     this->currentRoute->printAll();
                        // }
                        this->currentRoute = solution->routes.back();
                        this->currentNode = solution->plant;
                        // this->currentRoute->printAll();
                        break;
                    }
                    solution->addRoute(i+1);
                    this->currentRoute = solution->routes.back();
                    this->currentNode = solution->plant;
                    // this->currentRoute->printAll();
                    if (checkUsage(slack,i,solution) && i < 2){
                        this->currentRoute->type = i+1;
                        // std::cout << "cambio forzado" << '\n';
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
    //cout << "\n\ndisponible de "<< index<< " para recolectar: "<< available << '\n';
    // cout << "disponible en el camión: "<< this->currentRoute->remainingCapacity << '\n';
    // cout << "disponible en el camión con slack: "<< (this->currentRoute->remainingCapacity)*(1-slack) << "\n\n";
    // std::cout << ((this->currentRoute->remainingCapacity)*(1-slack) > available) << '\n';
    return ((this->currentRoute->remainingCapacity)*(1-slack) >= available);
}

vector<int> Construction::getRealDemand(Solution *solution){
    vector<int> available, real(solution->problemInstance->qualities.size());
    for (size_t i = 0; i < solution->problemInstance->qualities.size(); i++) {
        int s=0;
        for (Node *n: solution->problemInstance->NodesByType[i]) {
            s += n->getProduction();
        }
        // std::cout << "disponible de tipo "<< i << " "<< s << '\n';
        available.push_back(s);
    }
    for (size_t i = 0; i < solution->problemInstance->qualities.size(); i++) {
        if (solution->problemInstance->qualities[i] < available[i]){
            real[i] = solution->problemInstance->qualities[i];
        }
        else if(i == 1 && solution->problemInstance->qualities[i] > available[i]){
            real[i-1] += solution->problemInstance->qualities[i] - available[i];
            real[i] = available[i];
        }
        else if(i > 1 && solution->problemInstance->qualities[i-1] > available[i-1] && solution->problemInstance->qualities[i] > available[i]){
            real[i-2] = solution->problemInstance->qualities[i] - available[i];
            real[i] = available[i];
        }
        else{
            real[i] = solution->problemInstance->qualities[i];
        }
    }
    for (size_t i = 0; i < 3; i++) {
        // std::cout << "demanda de "<< i << " " << solution->problemInstance->qualities[i] << '\n';
        // std::cout << "demanda real de "<< i << " " << real[i] << '\n';
    }

    return real;
}
