#include "route.h"

Route::Route(MyData *ap) : list<int>(2, 0) // intialize to 0 -> 0
{
    appl = ap;
    cost = 0;
    time = 0;
    nbClients = 0;
}

Route::Route(Route const &R) : list<int>(R) {
    cost = R.cost;
    time = R.time;
    nbClients = R.nbClients;
}


// insert a customer at the end of the route, before returning to the depot
void Route::push_back_customer(int j) {
    int i;

    pop_back(); // remove the final depot
    i = back();
    push_back(j);
    push_back(0); //reinsert the final depot
    if (nbClients == 0) {
        cost = appl->getCost(0, j) + appl->getCost(j, 0);
        time = appl->getDistance(0, j) + appl->getServiceCost(j) + appl->getDistance(j, 0);
    }
    else {
        cost = cost - appl->getCost(i, 0) + appl->getCost(i, j) + appl->getCost(j, 0);
        time = time - appl->getDistance(i, 0) + appl->getDistance(i, j) + appl->getServiceCost(j) + appl->getDistance(j, 0);
    }
    nbClients++;
}

void Route::clear() {
    list<int>::clear();
    cost = 0;
    time = 0;
    nbClients = 0;
    push_back(0);
    push_back(0);
}


Route &Route::operator=(const Route &R2) {
    if (this == &R2)
        return *this;
    cost = R2.cost;
    time = R2.time;
    nbClients = R2.nbClients;
    (list<int>) *this = list<int>::operator=(R2);
    return *this;
}


ostream &operator<<(ostream &flot, Route &R) {
    list<int>::iterator iter;

    for (iter = R.begin(); iter != R.end(); ++iter)
        flot << *iter << " ";
    flot << " (cost: " << R.getCost() << ", time: " << R.getTime() << ")" << endl;
    return flot;
}
