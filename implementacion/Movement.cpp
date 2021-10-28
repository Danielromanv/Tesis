#include "Movement.h"

Movement::Movement():bestDistance(1000){};

Movement::~Movement()=default;

double Movement::ToptCheck(Solution *solution, int a, int b, Route * route){
    double org = route->distance, now = 0;
    std::cout << "realmente qué pasa?" << '\n';
    if (a == 0 || b >= route->trips.size()-1){
        return 0;
    }

    std::cout << "/**/ "<< route->trips[a-1]->initialNode->getId() << '\n';
    std::cout << "/**/ "<< route->trips[a-1]->finalNode->getId() << '\n';
    std::cout << "/**/ "<< route->trips[b+1]->initialNode->getId() << '\n';
    std::cout << "/**/ "<< route->trips[b+1]->finalNode->getId() << '\n';


    now -= (route->trips[a-1]->distance);
    now -= (route->trips[b+1]->distance);
    std::cout << "se le quita: "<< now << '\n';

    std::cout << "/**/ "<< route->trips[a-1]->initialNode->getId() << '\n';
    std::cout << "/**/ "<< route->trips[b-1]->finalNode->getId() << '\n';
    std::cout << "/**/ "<< route->trips[a-1]->finalNode->getId() << '\n';
    std::cout << "/**/ "<< route->trips[b]->finalNode->getId() << '\n';

    now += solution->problemInstance->getDistance(route->trips[a-1]->initialNode,route->trips[b-1]->finalNode);
    now += solution->problemInstance->getDistance(route->trips[a-1]->finalNode,route->trips[b]->finalNode);


    std::cout << "org: "<< org << '\n';
    std::cout << "now: "<< now << '\n';
    return now;

}

Route *Movement::createNewRoute(int desde, int hasta, Route *route, Solution * solution){
    int debug = 0;
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
