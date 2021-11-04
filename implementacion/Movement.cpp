#include "Movement.h"

Movement::Movement():bestDistance(1000){};

Movement::~Movement()=default;

double Movement::ToptCheck(Solution *solution, int a, int b, Route * route){
    double org = route->distance, now = 0;

    if (a > b){
        int aux = a;
        a = b;
        b = aux;
    }

    if (b >= route->trips.size()){
        std::cout << "really?" << '\n';
        std::cout << "a:"<< a<< "b: "<< b << '\n';
        std::cout << "tripsize"<< route->trips.size() << '\n';
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
    auto newRoute = new Route(route->getId(), route->truck, route->type);
    newRoute->remainingCapacity = route->remainingCapacity;
    newRoute->full = route->full;


    int i=0;
    int j=1;

    for(Trip *t: route->trips){ //Recorremos los trips que deben cambiar

        if(i == desde){
            //Este es el inicial.
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
