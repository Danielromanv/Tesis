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


void Movement::neigborhood(Solution *solution, Route * route){
    bool debug = 0;
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
                    if(abs(index1 - index2) > 2) {

                        if(this->ToptCheck(solution, index1, index2,route) < 0){
                            if (debug) cout << "check "<< ToptCheck(solution, index1, index2,route)  << '\n';
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

double * Movement::MovCheck(Solution *solution, int a, int b, int la, int lb, Route * routeA, Route * routeB, double punish){
    double Da=routeA->distance,Db=routeB->distance,Sa=0,Sb=0;
    int i = a, j = b, Ta=routeA->getTypeIndex(),Tb=routeB->getTypeIndex();
    static double r[2];
    if (a+la-1 > routeA->trips.size()-1 || b+lb-1 > routeB->trips.size()-1 || (a == 0 && b == 0)){
        r[0] = -1;
        r[1] = 3.141592653;
        return r;
    }
    // std::cout << "Ta: "<< Ta << '\n';
    // std::cout << "Tb: "<< Tb << '\n';
    while(i <= a+la-1 || j <= b+lb-1 ){
        if (i <= a+la-1) {
            Sa += routeA->trips[i]->finalNode->getProduction();
            // std::cout << "quito de A nodo "<< routeA->trips[i]->finalNode->getId() << '\n';
            Da -= routeA->trips[i]->distance;
            // std::cout << "Quito a A: "<< routeA->trips[i]->distance << '\n';
            Db += routeA->trips[i]->distance;
            if(routeA->trips[i]->finalNode->getTypeIndex() < Tb){
                Tb = routeA->trips[i]->finalNode->getTypeIndex();
            }
            i++;
        }
        if (j <= b+lb-1) {
            Sb += routeB->trips[j]->finalNode->getProduction();
            // std::cout << "quito de B nodo "<< routeB->trips[j]->finalNode->getId() << '\n';
            Db -= routeB->trips[j]->distance;
            if(j != b){Da += routeB->trips[j]->distance;
                // std::cout << "suma a A: "<< routeB->trips[j]->distance << '\n';
            }
            if(routeB->trips[j]->finalNode->getTypeIndex() < Ta){
                Ta = routeB->trips[j]->finalNode->getTypeIndex();
            }
            j++;
        }
    }
    // std::cout << "Sa: "<< Sa << '\n';
    // std::cout << "Sb: "<< Sb << '\n';
    // std::cout << "Da: "<< Da << '\n';
    // std::cout << "Db: "<< Db << '\n';
    // std::cout << "Ta: "<< Ta << '\n';
    // std::cout << "Tb: "<< Tb << '\n';
    Sa += routeA->trips[a-1]->finalNode->getProduction();
    Da += solution->problemInstance->getDistance(routeB->trips[b+lb-1]->finalNode,routeA->trips[a+la]->finalNode) + solution->problemInstance->getDistance(routeA->trips[a-1]->initialNode,routeB->trips[b]->finalNode) - routeA->trips[a-1]->distance - routeA->trips[a+la]->distance;
    Db += solution->problemInstance->getDistance(routeA->trips[a+la-1]->finalNode,routeB->trips[b+lb]->finalNode) + solution->problemInstance->getDistance(routeB->trips[b-1]->finalNode,routeA->trips[a]->initialNode) - routeB->trips[b+lb]->distance;
    // if(a == 0 && b != 0){
    //     Da += solution->problemInstance->getDistance(routeB->trips[b]->finalNode,routeA->trips[a+a+la]->finalNode) + solution->problemInstance->getDistance(routeA->trips[a]->initialNode,routeB->trips[b]->finalNode) - routeB->trips[b]->distance;
    //     Db += solution->problemInstance->getDistance(routeB->trips[b-1]->finalNode,routeA->trips[a]->finalNode);
    //     std::cout << "a" << '\n';
    // }
    // if(b == 0 && a != 0){
    //     Da += solution->problemInstance->getDistance(routeA->trips[a]->initialNode,routeB->trips[b]->finalNode) + solution->problemInstance->getDistance(routeB->trips[b]->finalNode,routeA->trips[a+1]->finalNode)  - routeA->trips[a+1]->distance ;
    //     Db += solution->problemInstance->getDistance(routeB->trips[b]->initialNode,routeA->trips[a]->finalNode) + solution->problemInstance->getDistance(routeA->trips[a]->finalNode,routeB->trips[b+1]->finalNode)  - routeA->trips[a]->distance - routeB->trips[b+1]->distance;
    //     std::cout << "b" << '\n';
    // }
    if ((routeA->remainingCapacity + Sa)>=Sb && (routeB->remainingCapacity + Sb)>=Sa){r[0] = 1;}
    else{r[0] = -1;}
    double ev = solution->PunishEvaluate(punish), ev2 = this->MovEvaluate(solution, punish, routeA, routeB,Sa,Sb,Da,Db,Ta,Tb);
    std::cout << "Ev original: "<< ev << '\n';
    std::cout << "Ev cambio: "<< ev2 << '\n';

    std::cout << "Cambios en Distancia A: Se quita "<< routeA->distance << " se suma "<< Da << " Se quita de leche "<< Sa << " Y se agrega " << Sb << '\n';
    std::cout << "Cambios en Distancia B: Se quita "<< routeB->distance << " se suma "<< Db << " Se quita de leche "<< Sb << " Y se agrega " << Sa << '\n';
    r[1] = ev2-ev;
    return r;

}

double Movement::MovEvaluate(Solution *solution,double punish, Route * routeA, Route * routeB, double Sa, double Sb, double Da, double Db, int Ta, int Tb){
    double totalDistance(0), recollected[solution->problemInstance->qualities.size()];
    memset (recollected, 0, sizeof(recollected));
    for (Route *r: solution->routes) {
        if ((r != routeA) && (r != routeB)){
            recollected[r->getTypeIndex()] += r->truck->getTotalCapacity()- r->remainingCapacity;
            totalDistance += r->distance;
        }
    }
    totalDistance += Da+Db;
    recollected[Ta] += routeA->truck->getTotalCapacity() - routeA->remainingCapacity - Sa + Sb;
    recollected[Tb] += routeB->truck->getTotalCapacity() - routeB->remainingCapacity - Sb + Sa;
    vector<double> v;
    double a;
    for (int i = 0; i < solution->recollected.size(); i++) {
        a = solution->problemInstance->qualities[i] - recollected[i];
        if (a >= 0 && solution->problemInstance->qualities[i]!=0){
            v.push_back((double)a/(double)solution->problemInstance->qualities[i]);
        }
        else{
            v.push_back(.0);
        }
    }
    //std::cout << "distancia: "<< totalDistance << '\n';
    double milk(0);
    for (int i=0; i < solution->recollected.size(); i++){
        //std::cout << "agregando: "<< (double)recollected[i] * solution->literCost[i] << '\n';
        milk += (double)recollected[i] * solution->literCost[i];
    }
    //std::cout << "eval distance"<< totalDistance << '\n';
    //std::cout << "milk: "<< v[0]<< " "<< v[1]<< v[2] << '\n';
    // std::cout << v[0] << '\n';
    // std::cout << (v[0]*solution->literCost[0]*4) << '\n';
    //std::cout << "obj castigado: "<< milk - (totalDistance * solution->kilometerCost) - punish*((v[2]*solution->literCost[2]*2) + (v[1]*solution->literCost[1]*3) + (v[0]*solution->literCost[0]*4)) << '\n';
    return milk - (totalDistance * solution->kilometerCost) - punish*((v[2]*solution->literCost[2]*2) + (v[1]*solution->literCost[1]*3) + (v[0]*solution->literCost[0]*4));
}
