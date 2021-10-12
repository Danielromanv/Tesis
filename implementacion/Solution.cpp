#include "Solution.h"
#include "algorithm"

bool Solution::DetectWrong(void){
    for (Route *r: this->routes) {
        if (r->DetectWrong()){
            r->printAll();
            getchar();
            return true;
        }
    }
    return false;
}

Solution::Solution(ProblemInstance *problemInstance, unsigned int seed){
    for(int i = 0; i < problemInstance->getNumberOfQualities(); ++i){
        this->recollected.push_back(0);
    }

    this->unsatisfiedDemand = problemInstance->qualities;

    for (Truck *truck: problemInstance->trucks){
        Truck *copy = truck;
        addTruck(copy);//modificar el tipo de ruta para indicar que es un estado inicial
    }

    for (Node * node: problemInstance->nodes){
        Node *copy = node;
        this->addNode(copy);
    }

    setQualities(problemInstance->getNumberOfQualities());

    this->addRoute(1);

    this->problemInstance = problemInstance;

    this->plant = new Node(0, 0, 0);
    this->literCost = {0.03, 0.021, 0.009};
    this->kilometerCost = 1;
    this->seed = seed;

}

Solution::~Solution() {
//    cout << "Deleting Solution" << endl;
    this->recollected.clear();
    this->recollected.shrink_to_fit();

    this->unsatisfiedDemand.clear();
    this->unsatisfiedDemand.shrink_to_fit();

    this->unusedTrucks.clear();
    this->unusedTrucks.shrink_to_fit();

    this->unvisitedNodes.clear();
    this->unvisitedNodes.shrink_to_fit();

    this->nodesXQuality.clear();
    this->nodesXQuality.shrink_to_fit();

    for (Route *route:this->routes) {
        delete route;
    }
    this->routes.clear();
    this->routes.shrink_to_fit();

    this->problemInstance = nullptr;

//    delete (this->plant);
//    this->plant = nullptr;

    this->literCost.clear();
    this->literCost.shrink_to_fit();
}

Solution::Solution(const Solution &s2){

    recollected = s2.recollected;
    unsatisfiedDemand = s2.unsatisfiedDemand;

    for (Truck *truck: s2.unusedTrucks){
        Truck *copy = truck;
        unusedTrucks.push_back(copy);
    }

    for (Node *node: s2.unvisitedNodes){
        Node *copy = node;
        unvisitedNodes.push_back(copy);
    }

    nodesXQuality = s2.nodesXQuality;

    for (Route *route: s2.routes){
        auto *copy = new Route(*route);
        routes.push_back(copy);
    }

    problemInstance = s2.problemInstance;

    plant = new Node(0, 0, 0);
    literCost = s2.literCost;
    kilometerCost = s2.kilometerCost;

    seed = s2.seed;
}

double Solution::random_number(double min, double max){
    double f = (double)rand() / RAND_MAX;
    double randomNumber = min + f * (max - min);
//    thread_local static mt19937 mt(this->seed);
//    thread_local static uniform_real_distribution<double> dist;
//    using pick = uniform_real_distribution<double>::param_type;
//    double randomNumber = dist(mt, pick(min, max));
    return randomNumber;
}

int Solution::random_int_number(int min, int max){
    int randomNumber = rand()%(max-min + 1) + min;
//    thread_local static mt19937 mt(this->seed);
//    thread_local static uniform_int_distribution<int> dist;
//    using pick = uniform_int_distribution<int>::param_type;
//    int randomNumber = dist(mt, pick(min, max));
    return randomNumber;
}

void Solution::setQualities(double size) {
    vector<int> aux(size, 0);
    for (Node *node: this->unvisitedNodes) {
        aux[node->getTypeIndex()]++;
    }
    for (int i = 0; i < size + 1; ++i) {
        int init = 0;
        if (i != 0) {
            this->nodesXQuality.push_back(accumulate(&aux[0], &aux[i], init));
        }
    }
}


void Solution::addNode(Node *node){this->unvisitedNodes.push_back(node);}

void Solution::addTruck(Truck *truck){this->unusedTrucks.push_back(truck);}

void Solution::addRoute(int type) {
    if (!this->unusedTrucks.empty()) {
        int routeId((int)this->routes.size() + 1);
        Truck *truck = this->getNextTruck();
        auto route = new Route(routeId, truck, type);
        this->routes.push_back(route);
        this->removeTruck(truck);
    }
}

void Solution::addTrip(Trip *trip, Route *route) {
    route->trips.push_back(trip);
}

void Solution::insertNode(Node *node) {
    this->unvisitedNodes.insert(this->unvisitedNodes.begin() + this->nodesXQuality[node->getTypeIndex()], node);
    for (int i = node->getTypeIndex(); i < this->nodesXQuality.size(); ++i) {
        this->nodesXQuality[i] += 1;
    }
}

void Solution::removeNode(Node *node) {
    auto it = find(this->unvisitedNodes.begin(), this->unvisitedNodes.end(), node);
    long index = std::distance(this->unvisitedNodes.begin(), it);
    this->unvisitedNodes.erase(this->unvisitedNodes.begin() + index);
    for (int i = node->getTypeIndex(); i < this->nodesXQuality.size(); ++i) {
        if (nodesXQuality[i] != 0) {
            this->nodesXQuality[i] -= 1;
        }
    }
}

void Solution::removeTruck(Truck *truck) {
    auto it = find(this->unusedTrucks.begin(), this->unusedTrucks.end(), truck);
    long index = std::distance(this->unusedTrucks.begin(), it);
    this->unusedTrucks.erase(this->unusedTrucks.begin() + index);
}
void Solution::insertTrip(Route *route, int index, Node *node) {
    Trip *modifiedNode = route->trips[index];
    route->distance -= modifiedNode->distance;
    Trip *newTrip = this->newTrip(modifiedNode->initialNode, node, route);

    modifiedNode->initialNode = node;
    modifiedNode->distance = this->problemInstance->getDistance(modifiedNode->initialNode, modifiedNode->finalNode);

    route->distance += modifiedNode->distance + newTrip->distance;
    route->remainingCapacity -= newTrip->finalNode->getProduction();

    route->trips.insert(route->trips.begin() + index, newTrip);
}


Trip *Solution::newTrip(Node *node1, Node *node2, Route *route) {
    int distance(problemInstance->getDistance(node1, node2));
    auto trip = new Trip(node1, node2, distance, node2->getType());
    trip->setRouteId(route->getId());
    return trip;
}

Node *Solution::getCurrentNode() {
    if (this->routes.back()->trips.empty()) {
        return this->plant;
    }
    return routes.back()->trips.back()->finalNode;
}

// entrega el camion mas grande
Truck *Solution::getNextTruck() {
    return *max_element(this->unusedTrucks.begin(), this->unusedTrucks.end());
}

void Solution::stepUpdateSolution(Trip *trip, Route *currentRoute, bool repairing) {
    currentRoute->distance += trip->distance;
    if (trip->finalNode != this->plant ) {
        int tripProduction(trip->finalNode->getProduction());
        this->recollected[trip->finalNode->getTypeIndex()] += tripProduction;
        currentRoute->remainingCapacity -= tripProduction;
        if (currentRoute->remainingCapacity == 0) {
            currentRoute->setFull();
        }
        updateDemands(currentRoute->getTypeIndex(), trip->finalNode, currentRoute, tripProduction, repairing);
        removeNode(trip->finalNode);
    }
}

void Solution::updateDemands(int currentTypeIndex, Node *currentNode, Route *currentRoute, int production, bool repairing) {
    int subtracting = production;
    bool pos(false);
    for (int i = currentTypeIndex; i < this->unsatisfiedDemand.size(); ++i) {
        int aux(getDemandSubtraction(this->unsatisfiedDemand[i], subtracting));
        if (this->unsatisfiedDemand[i] > 0 || currentRoute->isFull()) {
            pos = true;
        }
        this->unsatisfiedDemand[i] -= subtracting;
        subtracting = aux;
        addBackToPlant(i, currentNode, currentRoute, pos, repairing);
    }
}

void Solution::printAll() {
    cout << endl;
    cout << "**Solution**" << endl;

    double totalRecollected(0);
    cout << "recollected x quality: " << endl;
    for (int r: this->recollected) {
        totalRecollected += r;
        cout << r << endl;
    }

    cout << "Demand satisfaction: " << endl;
    for (int d: this->unsatisfiedDemand) {
        cout << d << endl;
    }

    cout << "# Unused trucks: " << this->unusedTrucks.size() << endl;
    for (Truck *t: this->unusedTrucks) {
        t->printAll();
    }

    cout << "# Unvisited nodes: " << this->unvisitedNodes.size() << endl;
    for (Node *n: this->unvisitedNodes) {
        n->printAll();
    }

    cout << "# nodes x quality: " << endl;
    for (int q: this->nodesXQuality) {
        cout << q << endl;
    }

    double totalDistance(0);
    for (Route *r: this->routes) {
        totalDistance += r->distance;
    }
    cout << "Distance cost: " << this->kilometerCost * totalDistance << endl;

    cout << "Milk cost: " << endl;
    double suma(0);
    for (int i = 0; i < this->recollected.size(); ++i) {
        suma += this->recollected[i] * this->literCost[i];
        cout << this->recollected[i] * this->literCost[i] << endl;
    }
    cout << "Total Milk cost: " << suma << endl;

    for (Route *r: this->routes) {
        cout << endl;
        r->printAll();
    }
}

char Solution::getType(int i, bool reverse){
    if (reverse){
        if(i == 0){
            return 'C';
        }
        if(i == 1){
            return 'B';
        }
        if(i == 2){
            return 'A';
        }
    }else {
        if (i == 2) {
            return 'C';
        }
        if (i == 1) {
            return 'B';
        }
        if (i == 0) {
            return 'A';
        }
    }
}

int Solution::getUnsatisfiedType(int from) { //TODO esta bien?
    for (int i = from ; i < this->unsatisfiedDemand.size() ; ++i) {
        if (this->unsatisfiedDemand[i] > 0) {
            //return random_int_number(from, i);//ELI
            return i;
        }
    }
    return -1; //si el ultimo tipo ya se suplio, devuelve -1
}

int Solution::getDemandSubtraction(int dda, int prod) {
    if (dda > 0) {
        if (dda - prod > 0) {
            return 0;
        } else {
            return -(dda - prod);
        }
    } else {
        return prod;
    }
}

void Solution::addBackToPlant(int i, Node *currentNode, Route *currentRoute, bool pos, bool repairing){
    if (currentRoute->trips.back()->finalNode != this->plant){

        if (this->unsatisfiedDemand[i] < 0 and pos) { // vuelve a planta si ya suplio la demanda
            Trip *toPlant = newTrip(currentNode, this->plant, currentRoute);
            currentRoute->distance += toPlant->distance;
            addTrip(toPlant, currentRoute);
            int actual(getUnsatisfiedType(0));
            // if ((!repairing) and (actual != -1)) { // nueva ruta para el tipo de dda no satisfecho
            //     addRoute(actual+1);
            // }
        }
    }
}

void Solution::printSolution() {
    cout << "Cantidad mínima a recoger (litros): " << endl;
    for (int d: this->problemInstance->qualities) {
        cout << d << endl;
    }

    cout << "Flota Camiones: " << endl;
    cout << "Nro. Cam.: " << this->problemInstance->trucks.size() << endl;
    cout << "Capacidad (litros): " << this->problemInstance->trucks[0]->getTotalCapacity() << endl;

    cout << "Beneficio unitario ($/litro)" << endl;
    for (double i : this->literCost) {
        cout << i << endl;
    }

    cout << endl << "Resultados:" << endl;

    for(Route *r: this->routes){
        cout << endl << "Muestra Ruta Ordenada " << r->getId() << " (" << r->distance << "):" << endl;
        for (Trip *t: r->trips){
            cout << t->finalNode->getId() << " ("<< t->distance <<") ";
        }
    }
    cout << endl;

    double totalDistance(0);
    for (Route *r: this->routes) {
        totalDistance += r->distance;
    }

    vector<int> litersxtype(3, 0); //TODO a la mala el 3
    for (Route *r: this->routes) {
        int liters = this->problemInstance->trucks[r->truck->getId() - 1]->getTotalCapacity() - r->remainingCapacity;
        cout << liters << " litros de leche " << r->type << " llegan en Camion " << r->truck->getId() << endl;
        litersxtype[r->getTypeIndex()] += liters;
    }

    for (int i =0 ; i < this->recollected.size(); i++) {
        cout << "En total llegan " << litersxtype[i] << " litros leche tipo " << i << " la planta" << endl;
    }
    this->recollected = litersxtype; // en algun momento lo calcula mal

    cout << endl << "CALCULO DE TOTAL MILK" << endl;
    double suma(0);

    cout << endl;
    cout << "COSTO_TPTE = " << totalDistance << endl;
    cout << "INGRESO_LECHE = " << to_string(suma) << endl;

}