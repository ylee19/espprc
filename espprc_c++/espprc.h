#pragma once

using namespace std;

#include "my_data.h"
#include "label.h"
#include "list_label.h"


//It constructed using the data (MyData object)
//The result is given by a list of routes

class ESPPRC {
public:
    ESPPRC(MyData *ap);

    ~ESPPRC();

    // solution method
    int solve();

    list<Route> *getRouteList() { return &routeList; }

	void processColinfo(list<Route> rList);
private:
    MyData *appl;
    double **distance;
    int nbNodes;
    // list of routes obtained after solving the pricing problem
    list<Route> routeList;
    // array of list of labels (one list for each vertex)
    ListLabel *labelsNode;
    // graph (list of successors for each vertex)
    list<int> *succ;

    int extendLabels(bool *pendingVertices, int current);

    int selectGraph();
};
