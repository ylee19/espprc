#pragma once

#include <list>
#include <fstream>
#include <iostream>
#include <cmath>
#include <string>
#include <iomanip>

using namespace std;

//  This class gives access to the data that describes the instance.

class MyData {
public :
    MyData();
	MyData(double **z_dual, double *y_dual, int **sc, int **st, int CAPA, int nbClients);

    MyData(string f, int nbClients);

    ~MyData();

    int getNbOfNodes() { return nbNodes; }

    int getNbOfVehicles() { return nbVehicles; }

    int getCapacity() { return capacity; }

    double *getStartWindow() { return startTW; }

    double *getEndWindow() { return endTW; }

    double *getQuantity() { return quantity; }

    double *getServiceCost() { return serviceCost; }

    double getStartWindow(int i) { return startTW[i]; }

    double getEndWindow(int i) { return endTW[i]; }

    double getQuantity(int i) { return quantity[i]; }

    double getServiceCost(int i) { return serviceCost[i]; }

    double **getDistances() { return distance; }

    double getDistance(int i, int j) { return distance[i][j]; }

    double **getCosts() {return cost;}

    double getCost(int i, int j) {return cost[i][j];}

    // array of list of successor nodes in the graph
    list<int> *getSuccessors() { return successors; }

    double getMyZero() { return myzero; }

    long getInfinity() { return infinity; }

    // string that names the instance
    string getFile() { return file; }

    void printEdges();

protected :
    double myzero;
    long infinity;
    int nbNodes;
    int nbVehicles;
    int capacity;
    // start of time windows
    double *startTW;
    // end of time windows
    double *endTW;
    // demand of customer
    double *quantity;
    // service time
    double *serviceCost;
    // distance matrix
    double **distance;
    // cost matrix
    double **cost;
    // array of lists of successor nodes
    list<int> *successors;
    // name of the instance file
    string file;

    void successorList();

    void readData();

};