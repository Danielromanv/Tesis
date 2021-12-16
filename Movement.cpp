#include "Movement.h"

Movement::Movement():bestDistance(1000),bestIndex(0){this->node = nullptr;};

Movement::~Movement()= default;

double Movement::ToptCheck(Solution *solution, int a, int b, Route * route){
    double org = route->distance, now = 0;

    if (a > b){
        int aux = a;
        a = b;
        b = aux;
    }

    if (b >= route->trips.size() || a == 0 || b == 0 || route->distance == 0){
        return 0;
    }

    now -= (route->trips[a]->distance);
    now -= (route->trips[b]->distance);

    now += solution->problemInstance->getDistance(route->trips[a]->initialNode,route->trips[b-1]->finalNode);
    now += solution->problemInstance->getDistance(route->trips[a]->finalNode,route->trips[b]->finalNode);

    return now;

}

Route *Movement::createNewRoute(int desde, int hasta, Route *route, Solution * solution){
    bool debug = 0;
    if (desde > hasta){
        int aux = desde;
        desde = hasta;
        hasta = aux;
    }
    // std::cout << "RUTA ORIGINAL" << '\n';
    // route->printAll();
    auto newRoute = new Route(route->getId(), route->truck, route->type);
    newRoute->remainingCapacity = route->remainingCapacity;
    newRoute->full = route->full;


    int i=0;
    int j=1;

    for(Trip *t: route->trips){ //Recorremos los trips que deben cambiar

        if(i == desde){
            //Este es el inicial.
            //std::cout << "se agrega: "<< t->initialNode->getId()<< " -> "<< route->trips[hasta]->initialNode->getId() << '\n';
            auto trip = new Trip(t->initialNode, route->trips[hasta]->initialNode, solution->problemInstance->getDistance(t->initialNode, route->trips[hasta]->initialNode), route->trips[hasta]->initialNode->getType());
            trip->routeId = route->getId();
            newRoute->distance += trip->distance;

            if (debug) t->printAll();
            if (debug) trip->printAll();
            if (debug) getchar();

            newRoute->trips.push_back(trip);

        }
        else if(desde < i && i < hasta){

            //Estos son los que se invierten
            // std::cout << "se agrega: "<< route->trips[hasta-j]->finalNode->getId()<< " -> "<< route->trips[hasta-j]->initialNode->getId() << '\n';
            auto trip = new Trip(route->trips[hasta-j]->finalNode, route->trips[hasta-j]->initialNode, solution->problemInstance->getDistance(route->trips[hasta-j]->finalNode, route->trips[hasta-j]->initialNode), route->trips[hasta-j]->initialNode->getType());
            trip->routeId = route->getId();
            newRoute->distance += trip->distance;


            if (debug) t->printAll();
            if (debug) trip->printAll();
            if (debug) getchar();

            newRoute->trips.push_back(trip);
            j++;
        }
        else if(i == hasta){
            //Este es el final.
            // std::cout << "se agrega: "<< route->trips[desde]->finalNode->getId()<< " -> "<< t->finalNode->getId() << '\n';
            auto trip = new Trip(route->trips[desde]->finalNode, t->finalNode, solution->problemInstance->getDistance(route->trips[desde]->finalNode, t->finalNode), t->finalNode->getType());

            trip->routeId = route->getId();
            newRoute->distance += trip->distance;


            if (debug) t->printAll();
            if (debug) trip->printAll();
            if (debug) getchar();

            newRoute->trips.push_back(trip);
        }
        else{
            //Estos se quedan iguales.
            //std::cout << "se agrega: "<< t->initialNode->getId()<< " -> "<<  t->finalNode->getId() << '\n';
            auto trip = new Trip(t->initialNode, t->finalNode, t->distance, route->getType());

            trip->routeId = route->getId();
            newRoute->distance += trip->distance;

            if (debug) t->printAll();
            if (debug) trip->printAll();
            if (debug) getchar();
            newRoute->trips.push_back(trip);
        }
        i++;
    }

    if (debug) newRoute->printAll();
    if (debug) getchar();

    return newRoute;
}


void Movement::ToptNeigborhood(Solution *solution, Route * route){
    bool debug = false;
    int size_ruta=route->trips.size();
    if (size_ruta < 3) return;
    int start1=solution->random_int_number(0,size_ruta-1);
    int tries1=0;
    while(tries1 < size_ruta){
        int index1=(start1+tries1)%size_ruta;
        if (index1 < size_ruta){
            int start2=solution->random_int_number(0, size_ruta-1);
            int tries2=0;
            while(tries2 < size_ruta){
                int index2=(start2+tries2)%size_ruta;
                if (index2 < size_ruta){
                    if(abs(index1 - index2) > 1) {

                        if(this->ToptCheck(solution, index1, index2,route) < 0){
                            if (debug) cout << "check: "<< ToptCheck(solution, index1, index2,route)  << '\n';
                            if (debug) cout << "index 1: "<< index1<< " index 2: "<< index2 << '\n';
                            Route *newRoute = this->createNewRoute(index1, index2, route, solution);

                            if (debug)  cout << "route->distance: " << route->distance << endl;

                            if (debug) cout << "newRoute->distance: " << newRoute->distance << endl;

                            if (debug) route->printAll();
                            if (debug) newRoute->printAll();

                            if(newRoute != nullptr){
                                solution->replaceInSolution(newRoute);
                                return;
                            }
                            delete newRoute;
                        }
                    }
                }
                tries2++;
            }
        }
        tries1++;
    }
    return;
}

double * Movement::ExCheck(Solution *solution, int a, int b, int la, int lb, Route * routeA, Route * routeB, double punish){
    double Da=routeA->distance,Db=routeB->distance,Sa=0,Sb=0;
    int i = a, j = b, Ta=routeA->getTypeIndex(),Tb=routeB->getTypeIndex();
    static double r[2];
    if ((a+la >= routeA->trips.size()) || (b+lb >= routeB->trips.size()) || la == 0 || lb == 0){
        r[0] = 0;
        return r;
    }
    // std::cout << "Ta: "<< Ta << '\n';
    // std::cout << "Tb: "<< Tb << '\n';
    while(i <= a+la-1 || j <= b+lb-1 ){
        if (i <= a+la-1) {
            Sa += routeA->trips[i]->finalNode->getProduction();
            // std::cout << "quito de A nodo "<< routeA->trips[i]->finalNode->getId() << '\n';
            Da -= routeA->trips[i]->distance;
            // std::cout << "quito dist de A "<< routeA->trips[i]->distance << '\n';
            if(i != a){
            Db += routeA->trips[i]->distance;
            // std::cout << "Sumo a B: "<< routeA->trips[i]->distance << '\n';
            }
            if(routeA->trips[i]->finalNode->getTypeIndex() > Tb){
                Tb = routeA->trips[i]->finalNode->getTypeIndex();
            }
            i++;
        }
        if (j <= b+lb-1) {
            Sb += routeB->trips[j]->finalNode->getProduction();
            // std::cout << "quito de B nodo "<< routeB->trips[j]->finalNode->getId() << '\n';
            Db -= routeB->trips[j]->distance;
            // std::cout << "quito dist de B "<< routeB->trips[j]->distance << '\n';
            if(j != b){
                Da += routeB->trips[j]->distance;
                // std::cout << "suma a A: "<< routeB->trips[j]->distance << '\n';
            }
            if(routeB->trips[j]->finalNode->getTypeIndex() > Ta){
                Ta = routeB->trips[j]->finalNode->getTypeIndex();
            }
            j++;
        }
        if (i == a+la){
            Da -= routeA->trips[i]->distance;
            // std::cout << "quito dist de A: "<< routeA->trips[i]->distance << '\n';
            i++;
        }
        if (j == b+lb){
            Db -= routeB->trips[j]->distance;
            // std::cout << "quito dist de B: "<< routeB->trips[j]->distance << '\n';
            j++;
        }
    }

    Da += solution->problemInstance->getDistance(routeB->trips[b+lb-1]->finalNode,routeA->trips[a+la]->finalNode)+solution->problemInstance->getDistance(routeA->trips[a]->initialNode,routeB->trips[b]->finalNode);
    Db += solution->problemInstance->getDistance(routeA->trips[a+la-1]->finalNode,routeB->trips[b+lb]->finalNode)+solution->problemInstance->getDistance(routeB->trips[b]->initialNode,routeA->trips[a]->finalNode);

    if ((routeA->remainingCapacity + Sa)>=Sb && (routeB->remainingCapacity + Sb)>=Sa){r[0] = 1;}
    else{r[0] = -1;}
    double ev = solution->PunishEvaluate(punish)[0], ev2 = this->ExEvaluate(solution, punish, routeA, routeB,Sa,Sb,Da,Db,Ta,Tb);
//     if (ev2-ev > 0 && r[0] == 1){
//     std::cout << "Cambios en Distancia B: Se quita "<< routeB->distance << " se suma "<< Db << " Se quita de leche "<< Sb << " Y se agrega " << Sa << '\n';
//     std::cout << "Cambios en Distancia A: Se quita "<< routeA->distance << " se suma "<< Da << " Se quita de leche "<< Sa << " Y se agrega " << Sb << '\n';
//     std::cout << "ev1: "<< ev << "ev2: "<< ev2 << '\n';
// }

    r[1] = ev2-ev;
    return r;

}

double Movement::ExEvaluate(Solution *solution,double punish, Route * routeA, Route * routeB, double Sa, double Sb, double Da, double Db, int Ta, int Tb){
    double totalDistance(0);
    vector<int> recollected(solution->problemInstance->qualities.size());
    for (Route *r: solution->routes) {
        if ((r->getId() != routeA->getId()) && (r->getId() != routeB->getId())){
            recollected[r->getTypeIndex()] += r->truck->getTotalCapacity()- r->remainingCapacity;
            totalDistance += r->distance;
        }
    }
    totalDistance += Da+Db;
    recollected[Ta] += routeA->truck->getTotalCapacity() - routeA->remainingCapacity - Sa + Sb;
    recollected[Tb] += routeB->truck->getTotalCapacity() - routeB->remainingCapacity - Sb + Sa;
    vector<int> realrecollect = solution->newRecollected(recollected,0);
    vector<double> v;
    double a;
    for (int i = 0; i < realrecollect.size(); i++) {
        a = solution->problemInstance->qualities[i] - realrecollect[i];
        if (a >= 0 && solution->problemInstance->qualities[i]!=0){
            v.push_back((double)a/(double)solution->problemInstance->qualities[i]);
        }
        else{
            v.push_back(.0);
        }
    }
    //std::cout << "distancia: "<< totalDistance << '\n';
    double milk(0);
    for (int i=0; i < realrecollect.size(); i++){
        //std::cout << "agregando: "<< (double)realrecollect[i] * solution->literCost[i] << '\n';
        milk += (double)realrecollect[i] * solution->literCost[i];
    }

    return milk - (totalDistance * solution->kilometerCost) - (punish*((v[2]*solution->literCost[2]*MilkWeight[2]) + (v[1]*solution->literCost[1]*MilkWeight[1]) + (v[0]*solution->literCost[0]*MilkWeight[0])));
}

void Movement::ChangeTrip(Solution *solution, int a, int b, int la, int lb, Route * routeA, Route * routeB, double punish){
    bool debug = 0;
    int i = 0;
    Route * newRouteA, * newRouteB;
    //std::cout << "Check "<< ExCheck(solution, a, b, la, lb, routeA, routeB, punish)[0] << '\n';
    if(ExCheck(solution, a, b, la, lb, routeA, routeB, punish)[0] != 1){
        return;
    }

    solution->recollected[routeA->getTypeIndex()] -=routeA->truck->getTotalCapacity()- routeA->remainingCapacity;
    solution->recollected[routeB->getTypeIndex()] -=routeB->truck->getTotalCapacity()- routeB->remainingCapacity;


    newRouteA = this->ChangeRTrip(solution, a, b, la, lb, routeA, routeB, punish);
    newRouteB = this->ChangeRTrip(solution, b, a, lb, la, routeB, routeA, punish);


    solution->recollected[newRouteA->getTypeIndex()] +=newRouteA->truck->getTotalCapacity()- newRouteA->remainingCapacity;
    solution->recollected[newRouteB->getTypeIndex()] +=newRouteB->truck->getTotalCapacity()- newRouteB->remainingCapacity;
    solution->replaceInSolution(newRouteA);
    solution->replaceInSolution(newRouteB);

    return;
}

Route * Movement::ChangeRTrip(Solution *solution, int a, int b, int la, int lb, Route * routeA, Route * routeB, double punish){
    bool debug = 0;
    int i = 0;
    auto newRoute = new Route(routeA->getId(), routeA->truck, 0);
    newRoute->remainingCapacity = routeA->truck->getTotalCapacity();
    newRoute->full = routeA->full;
    newRoute->type = 0;
    for(Trip *t: routeA->trips){ //Recorremos los trips que deben cambiar

        if(i < a){
            //Esto son los anteriores al punto de cambio
            //std::cout << "se agrega: "<< t->initialNode->getId()<< " -> "<< route->trips[hasta]->initialNode->getId() << '\n';
            auto trip = new Trip(t->initialNode, t->finalNode, solution->problemInstance->getDistance(t->initialNode, t->finalNode), t->finalNode->getType());
            trip->routeId = routeA->getId();
            newRoute->distance += trip->distance;
            newRoute->remainingCapacity -= t->finalNode->getProduction();

            if (debug) t->printAll();
            if (debug) trip->printAll();
            //if (debug) getchar();

            if (t->finalNode->getType() > newRoute->getType()){
                newRoute->type =t->finalNode->getType();
            }
            newRoute->trips.push_back(trip);

        }
        if (i == a){
            //aquí se agrega un trip nuevo que incluye el incial de la ruta con el final inicial de la otra

            auto trip = new Trip(t->initialNode, routeB->trips[b]->finalNode, solution->problemInstance->getDistance(routeA->trips[a]->initialNode,routeB->trips[b]->finalNode), routeB->trips[b]->finalNode->getType());
            trip->routeId = routeA->getId();
            newRoute->distance += trip->distance;
            newRoute->remainingCapacity -= routeB->trips[b]->finalNode->getProduction();

            if (debug) t->printAll();
            if (debug) trip->printAll();
            //if (debug) getchar() << '\n';
            if (routeB->trips[b]->finalNode->getType() > newRoute->getType()){
                newRoute->type = routeB->trips[b]->finalNode->getType();
            }
            newRoute->trips.push_back(trip);
        }
        if (i == a+1){
            //std::cout << "aqui no se si agregar con un for todos los otros, y con otro indice" << '\n';
            if(lb > 1){
                for (size_t j = b+1; j < b+lb; j++) {
                    auto trip = new Trip(routeB->trips[j]->initialNode, routeB->trips[j]->finalNode, routeB->trips[j]->distance, routeB->trips[j]->quality);
                    trip->routeId = routeA->getId();
                    newRoute->distance += trip->distance;
                    newRoute->remainingCapacity -= routeB->trips[j]->finalNode->getProduction();

                    if (debug) t->printAll();
                    if (debug) trip->printAll();
                    //if (debug) getchar();

                    if (routeB->trips[j]->finalNode->getType() > newRoute->getType()){
                        newRoute->type = routeB->trips[j]->finalNode->getType();
                    }
                    newRoute->trips.push_back(trip);
                }
            }
        }
        if (i == a+la){
            auto trip = new Trip(routeB->trips[b+lb-1]->finalNode, t->finalNode, solution->problemInstance->getDistance(routeB->trips[b+lb-1]->finalNode, t->finalNode), t->finalNode->getType());
            trip->routeId = routeA->getId();
            newRoute->distance += trip->distance;
            newRoute->remainingCapacity -= t->finalNode->getProduction();

            if (debug) t->printAll();
            if (debug) trip->printAll();
            //if (debug) getchar();

            if (t->finalNode->getType() > newRoute->getType()){
                newRoute->type = t->finalNode->getType();
            }
            newRoute->trips.push_back(trip);
        }
        if(i > a+la){
            //Esto son los anteriores al punto de cambio
            //std::cout << "se agrega: "<< t->initialNode->getId()<< " -> "<< route->trips[hasta]->initialNode->getId() << '\n';
            auto trip = new Trip(t->initialNode, t->finalNode, solution->problemInstance->getDistance(t->initialNode, t->finalNode), t->finalNode->getType());
            trip->routeId = routeA->getId();
            newRoute->distance += trip->distance;
            newRoute->remainingCapacity -= t->finalNode->getProduction();

            if (debug) t->printAll();
            if (debug) trip->printAll();
            //if (debug) getchar();

            if (t->finalNode->getType() > newRoute->getType()){
                newRoute->type = t->finalNode->getType();
            }
            newRoute->trips.push_back(trip);
        }

        i++;
    }
    if (debug) routeA->printAll();
    if (debug) newRoute->printAll();
    return newRoute;
}

void Movement::ExNeiborhood(Solution *solution, Route * routeA, Route * routeB, double punish){
    bool debug = 0;
    int size_rutaA = routeA->trips.size(), size_rutaB = routeB->trips.size();
    double *ev;
    if (routeA->trips.size() == 0 ||routeB->trips.size() == 0){
        return;
    }
    for (size_t la = 1; la <= 5; la++) {
        /* code */
        //For de la ruta A, y recorro todos los largos posibles
        for (size_t lb = 1; lb <= 5; lb++) {
            //For de la ruta B, y recorro todos los largos posibles
            //random a y random b
            int startA=solution->random_int_number(0,size_rutaA-1);
            int triesA=0;
            while(triesA < size_rutaA){
                int indexA=(startA+triesA)%size_rutaA;
                if (indexA < size_rutaA){
                    int startB=solution->random_int_number(0,size_rutaB-1);
                    int triesB=0;
                    while(triesB < size_rutaB){
                        int indexB=(startB+triesB)%size_rutaB;
                        if (indexB < size_rutaB){
                            //if mejora (lo hago, los cambio en la solucion)
                            ev = this->ExCheck(solution, indexA, indexB, la, lb, routeA, routeB, punish);
                            if(ev[0] == 1 && ev[1] > 0){
                                if (debug){
                                    std::cout << "------ Exchange -------" << '\n';
                                    for (int i = indexA; i < indexA+la; i++) {
                                        routeA->trips[i]->printAll();
                                    }
                                    std::cout << "\n";
                                    for (int i = indexB; i < indexB+lb; i++) {
                                        routeB->trips[i]->printAll();
                                    }
                                    std::cout <<"excheck" <<ExCheck(solution, indexA, indexB, la, lb, routeA, routeB, punish)[1] << '\n';
                                    routeA->printAll();
                                    routeB->printAll();
                                    std::cout << "index: "<< indexA<<" la:"<< la << '\n';
                                    std::cout << "index: "<< indexB<<" lb:"<< lb << '\n';
                                    std::cout << solution->PunishEvaluate(punish)[0] << '\n';

                                }
                                int ra = routeA->getId()-1;
                                int rb = routeB->getId()-1;
                                this->ChangeTrip(solution, indexA, indexB, la, lb, routeA, routeB, punish);
                                if(debug){
                                    solution->routes[ra]->printAll();
                                    solution->routes[rb]->printAll();
                                    std::cout << "real: "<<solution->PunishEvaluate(punish)[0] << '\n';
                                }
                                return;
                            }
                            // y return
                            //cambios
                        }
                    triesB++;
                    }
                }
            triesA++;
            }
        }
    }
}

vector<Node *> Movement::getCandidates(Solution * solution, int a){
    vector<Node *> candidates;
    int selected,i=0;
    // for (Node *n: solution->unvisitedNodes){
    //     n->printAll();
    // }
    if (a > solution->unvisitedNodes.size()){
        a = solution->unvisitedNodes.size();
    }
    while (i < a) {
        selected = rand() % solution->unvisitedNodes.size();
        if (std::find(candidates.begin(),candidates.end(),solution->unvisitedNodes[selected]) == candidates.end()){
            candidates.push_back(solution->unvisitedNodes[selected]);
            i++;
        }
    }
    return candidates;
}

void Movement::RemoveFromRoute(Solution * solution, int a){
    bool debug = 0;
    int selected = rand() % (solution->routes[a]->trips.size()-1);
    if (debug){
        std::cout << "remove: "<< '\n';
        solution->routes[a]->trips[selected]->printAll();
    }
    solution->removeTrip(selected, solution->routes[a]);
}

void Movement::AddCandidates(Solution * solution, int max){
    bool debug = 0;
    vector<Node *> candidates = this->getCandidates(solution, solution->problemInstance->getNumberOfNodes());
    // vector<Node *> candidates = this->getCandidates(solution, solution->random_int_number(1,max));
    int routesize = solution->routes.size(), r;
    //if(rand() < 0.8){
    int route_id;
    if(solution->random_number(0,1) < 1.0){
        route_id = rand() % (solution->routes.size()-1);
        for (size_t i = 0; i < max; i++) {
            this->RemoveFromRoute(solution, route_id);
        }
    }
    int start1, tries1;
    for (size_t i = 0; i < candidates.size(); i++) {
        start1 = solution->random_int_number(0,routesize-1);
        tries1 = 0;
        while(tries1 < routesize){
            int index1=(start1+tries1)%routesize;
            if ((!solution->routes.size())||(solution->routes[index1]->remainingCapacity > candidates[i]->getProduction() && candidates[i]->getTypeIndex() <= solution->routes[index1]->getTypeIndex())){
                if(debug){
                    std::cout << "add canidate: " << '\n';
                    candidates[i]->printAll();
                    std::cout << "to route id and type"<< solution->routes[index1]->getId() <<" "<< solution->routes[index1]->getTypeIndex() << '\n';
                    solution->routes[index1]->printAll();
                }
                solution->insertTrip(solution->routes[index1],0,candidates[i]);
                if(debug)solution->routes[index1]->printAll();
                break;
            }
            else{
                if (solution->routes.size() == 0 && candidates.size()>0) solution->insertTrip(solution->routes[index1],0,candidates[solution->random_int_number(0,candidates.size()-1)]);
            }
            tries1++;
        }
    }
}

vector<double> Movement::checkRoute(Solution * solution, Route * route){
    int local[solution->problemInstance->qualities.size()], count = 0;
    vector<double> proportion(solution->problemInstance->qualities.size()+1,0);
    memset(local, 0, sizeof(local));
    for(Trip *t: route->trips){
        if (t->finalNode->getTypeIndex() >= 0){
            local[t->finalNode->getTypeIndex()] += t->finalNode->getProduction();
        }
    }
    for (size_t i = 1; i < proportion.size(); i++) {
        proportion[i] += (double)local[i-1]/(route->truck->getTotalCapacity() - route->remainingCapacity);
        if (proportion[i] > 0){
            count++;
        }
    }
    if (count > 0) proportion[0] = 1;
    return proportion;
}

// void Movement::purify(Solution * solution, Route * route){
//     vector<double> v = checkRoute(solution, route);
//     int j =0;
//     if (v[0]){
//         for (size_t i = 1; i < v.size(); i++) {
//             if(v[i] > 0){
//                 do {
//                     if(route->trips[j]->finalNode->getType() > i){
//                         solution->removeTrip(j, route);
//                     }
//                     else{
//                         j++;
//                     }
//                 }while(route->trips[j]->finalNode->getId() != 0);
//                 return;
//             }
//         }
//     }
// }

void Movement::purify(Solution * solution, Route * route){
    vector<double> v = checkRoute(solution, route);
    int j;
    if(v[0] > 0){
        for(int i=1; i<4; i++){
            j = 0;
            if(v[i] < 0.1){
                do {
                    if(route->trips[j]->finalNode->getType() == i){
                        solution->removeTrip(j, route);
                    }
                    else{
                        j++;
                    }
                }while(route->trips[j]->finalNode->getId() != 0);
            }
        }
    }
}
