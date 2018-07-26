#pragma once

#include<iostream>
#include<list>
#include<utility>
#include<cmath>

using namespace std;

#include"my_data.h"

class Route : public list<int> {
    friend ostream &operator<<(ostream &flot, Route &R);

public :
    Route(MyData *appl);

    Route(Route const &R);

    Route &operator=(const Route &R2);

    double getCost() const { return cost; }

    double getTime() const { return time; }

    int getNbOfCustomers() const { return nbClients; }

    void push_back_customer(int j);

    void clear();

protected :
    MyData *appl;
    double cost;
    double time;
    int nbClients;
};