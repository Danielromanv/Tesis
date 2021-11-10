Route *DosOpt::createNewRoute(int desde, int hasta, Route *route, Solution * solution) {
    bool debug = false;

    if (desde > hasta){
        int aux = desde;
        desde = hasta;
        hasta = aux;
    }

    if (debug) cout << "desde: "<< desde << ", hasta: " << hasta << endl;
//     route->printAll();
    auto newRoute = new Route(route->id, route->truck, route->type);

    newRoute->type = route->type; //mantengo tipo
    newRoute->cType = route->cType; //mantengo tipo critico

    newRoute->remainingCapacity = route->remainingCapacity; //mantengo capacidad
    newRoute->distance = 0.0; //re-calculo distancia
    newRoute->benefit = 0.0; //mantengo beneficio, NO cambia respecto a las distancias actualmente.
    newRoute->concentratedCells = route->concentratedCells; //mantengo concentración de células

    int i=0;
    int j=1;

    for(Trip *t: route->trips){ //Recorremos los trips que deben cambiar

        if(i == desde){
            //Este es el inicial.
            auto trip = new Trip(route->getId(), t->initialNode, route->trips[hasta]->initialNode, solution->problemInstance->getDistance(t->initialNode, route->trips[hasta]->initialNode), solution->problemInstance->getNode(route->trips[hasta]->initialNode)->getProduction(), solution->problemInstance->getNode(route->trips[hasta]->initialNode)->getType(), solution->problemInstance->getNode(route->trips[hasta]->initialNode)->getCells(), 0);

            trip->benefit=solution->calculateBenefit(trip);
            newRoute->benefit += trip->benefit;
            newRoute->distance += trip->distance;

            if (debug) t->printAll();
            if (debug) trip->printAll();
            if (debug) getchar();

            newRoute->trips.push_back(trip);

        }
        else if(desde < i && i < hasta){

            //Estos son los que se invierten
            auto trip = new Trip(route->getId(), route->trips[hasta-j]->finalNode, route->trips[hasta-j]->initialNode, solution->problemInstance->getDistance(route->trips[hasta-j]->finalNode, route->trips[hasta-j]->initialNode), solution->problemInstance->getNode(route->trips[hasta-j]->initialNode)->getProduction(), solution->problemInstance->getNode(route->trips[hasta-j]->initialNode)->getType(), solution->problemInstance->getNode(route->trips[hasta-j]->initialNode)->getCells(), 0);

            trip->benefit=solution->calculateBenefit(trip);
            newRoute->benefit += trip->benefit;
            newRoute->distance += trip->distance;

            if (debug) t->printAll();
            if (debug) trip->printAll();
            if (debug) getchar();

            newRoute->trips.push_back(trip);
            j++;
        }
        else if(i == hasta){
            //Este es el final.
            auto trip = new Trip(route->getId(), route->trips[desde]->finalNode, t->finalNode, solution->problemInstance->getDistance(route->trips[desde]->finalNode, t->finalNode), solution->problemInstance->getNode(t->finalNode)->getProduction(), solution->problemInstance->getNode(t->finalNode)->getType(), solution->problemInstance->getNode(t->finalNode)->getCells(), 0);

            trip->benefit=solution->calculateBenefit(trip);
            newRoute->benefit += trip->benefit;
            newRoute->distance += trip->distance;


            if (debug) t->printAll();
            if (debug) trip->printAll();
            if (debug) getchar();

            newRoute->trips.push_back(trip);
        }
        else{
            //Estos se quedan iguales.
            auto trip = new Trip(route->getId(), t->initialNode, t->finalNode, t->distance, t->production, route->getType(), t->cells, t->benefit);

            newRoute->benefit += trip->benefit;
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

void DosOpt::neigborhood(Solution *solution, Route * route, int route_index){
    bool debug = false;
    int size_ruta=route->trips.size();
    if (size_ruta == 0) return;

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
                    if(absolute(index1, index2) > 2) {

                        if(this->evaluate_change(solution, route, index1, index2) >= 0){

                            Route *newRoute = this->createNewRoute(index1, index2, route, solution);

                            if (debug)  cout << "route->computeBenefit(solution->problemInstance, solution->kilometerCost): " << route->computeBenefit(solution->problemInstance, solution->kilometerCost) << endl;

                            if (debug) cout << "newRoute->computeBenefit(solution->problemInstance, solution->kilometerCost): " << newRoute->computeBenefit(solution->problemInstance, solution->kilometerCost) << endl;

                            if (debug) route->printAll();
                            if (debug) newRoute->printAll();

                            if (debug) getchar();

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
