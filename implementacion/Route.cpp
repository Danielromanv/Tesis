#include "Route.h"

bool Route::DetectWrong(){
    if(this->distance == 0){
        return false;
    }
    if(this->trips.back()->finalNode->getId() != 0){
        std::cout << "ruta con problema 0" << '\n';
        this->printAll();
        getchar();
        return true;
    }
    else if(this->trips[0]->initialNode != this->trips.back()->finalNode){
        std::cout << "ruta con problema 1" << '\n';
        this->printAll();
        getchar();
        return true;
    }
    for (size_t i = 0; i < this->trips.size(); i++) {
        //que el nodo de la planta no puede estar en otra posicion que no sea el inicio o final
        if (i != 0 && i != this->trips.size()-1 && (this->trips[i]->initialNode->getId() == 0 || this->trips[i]->finalNode->getId() == 0)){
            std::cout << "ruta con problema 2" << '\n';
            this->printAll();
            getchar();
            return true;
        }
    }
    return false;
}

Route::Route(int id, Truck *truck, int type):
id(id), truck(truck), type(type), remainingCapacity(truck->getTotalCapacity()), full(false), distance(0){}

Route::~Route() {
    this->truck = nullptr;

    for (Trip *trip: this->trips) {
        delete trip;
    }
    this->trips.clear();
    this->trips.shrink_to_fit();
}

Route::Route(const Route &route){
    id = route.id;
    truck = route.truck;
    remainingCapacity = route.remainingCapacity;
    type = route.type;
    full = route.full;
    distance = route.distance;

    for(Trip* trip: route.trips){
        auto *copy = new Trip(*trip);
        trips.push_back(copy);
    }

}

int Route::getId(){ return this->id; }

int Route::getTypeIndex(){ return this->type - 1; }

int Route::getType(){ return this->type; }

bool Route::isFull(){ return this->full; }

void Route::setFull() { this->full = true; }

void Route::resetRouteType() {
    int newType(0);
    for (Trip *t: this->trips) {
        if (t->finalNode->getType() > newType) {
            newType = t->finalNode->getType();
        }
    }
    this->type = newType;
}

bool Route::fitsInTruck(Node *node){
    return this->remainingCapacity >= node->getProduction();
}

void Route::printAll() {
    double aux(0);
    for(Trip *trip: this->trips){
        aux += trip->benefit;
    }
    cout << "Route: " << this->getId() << "\ttruck: " << this->truck->getId() << "\tmilk type: " << this->type << "\tisFull: "
         << this->full << endl;
    cout << "Num. of trips: " << this->trips.size() << "\tremaining Capacity: " << this->remainingCapacity << "\tdistance: " << this->distance << endl;
    for(Trip *trip: this->trips){
        trip->printAll();
    }
}
