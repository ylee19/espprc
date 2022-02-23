#include "my_data.h"
#include <stdio.h>
#include <string>
#include <string.h>
#include <iostream>
#include <fstream>
MyData::MyData() {
    throw ("construction of an empty object MyData");
}

MyData::MyData(string f, int nbClients) {
    myzero = 0.001;
    infinity = 1000000;
    file = f;
    nbNodes = nbClients + 1;
    readData(); // read data and compute distance matrix
    successorList(); //compute the set of outgoing arcs for each node
}

MyData::MyData(double **z_dual, double *y_dual, int **sc, int **st, int CAPA, int nbClients) {
	myzero = 0.001;
	infinity = 1000000;
	
	nbNodes = nbClients + 1;
	readData(); // read data and compute distance matrix

	int i, j, n, x, y;
	

	distance = new double *[nbNodes];
	cost = new double *[nbNodes]; 
	for (i = 0; i < nbNodes; i++) {
		distance[i] = new double[nbNodes];
		cost[i] = new double[nbNodes];
	}
	
	startTW = new double[nbNodes];
	endTW = new double[nbNodes];
	quantity = new double[nbNodes];
	serviceCost = new double[nbNodes];
	successors = new list<int>[nbNodes];

	nbVehicles = 1;
	capacity = CAPA;
	
	for (i = 0; i < nbNodes; i++) {
		startTW[i] = 0; endTW[i] = capacity;
		quantity[i] = 0;	//Need check!
		serviceCost[i] = 0;//Nee check;
	}
	
	for (i = 0; i < nbNodes; i++)
		for (j = 0; j < nbNodes; j++)
			if (i == j) {
				cost[i][j] = MyData::infinity;
				distance[i][j] = MyData::infinity;
			}
			else if (i == 0) { // z_0j
				cost[i][j] = -z_dual[nbClients][j - 1] - y_dual[j - 1];
				distance[i][j] = 0;
			}
			else if (j==0) { // z_i0
				cost[i][j] = -z_dual[i-1][nbClients];
				distance[i][j] = 0;
			}
			else {	// z_ij
				cost[i][j] = sc[i - 1][j - 1] - z_dual[i - 1][j - 1] - y_dual[j - 1];
				distance[i][j] = st[i - 1][j - 1];
			}

	successorList(); //compute the set of outgoing arcs for each node
}
MyData::~MyData() {
    int i;
    delete[] successors;
    delete[] serviceCost;
    delete[] quantity;
    delete[] endTW;
    delete[] startTW;
    for (i = 0; i < nbNodes; i++)
        delete[] distance[i];
    delete[] distance;
}

void MyData::successorList() {
    int i, j;

    for (i = 0; i < nbNodes; i++)
        for (j = 1; j < nbNodes; j++)
            if (startTW[i] + serviceCost[i] + distance[i][j] <= endTW[j])
                successors[i].push_back(j);
    for (i = 1; i < nbNodes; i++)
        successors[i].push_back(0);

}

// read data from the file
void MyData::readData() {
    int i, j, n, x, y;
    int *X;
    int *Y;
    ifstream dataFile;

    distance = new double *[nbNodes];
    for (i = 0; i < nbNodes; i++)
        distance[i] = new double[nbNodes];

    cost = new double *[nbNodes];
    for (i = 0; i < nbNodes; i++)
        cost[i] = new double[nbNodes];

    startTW = new double[nbNodes];
    endTW = new double[nbNodes];
    quantity = new double[nbNodes];
    serviceCost = new double[nbNodes];
    successors = new list<int>[nbNodes];
	
    X = new int[nbNodes];
    Y = new int[nbNodes];
	
/*	ifstream infile(file.c_str());
	if (!infile.is_open()) {
		cout << "ERROR: Input file open failed: " << file << endl;
		exit(1);
	}*/
	//dataFile.open(file);
	dataFile.open("./instances/r101-100.dat");
    //dataFile.open(file.c_str());
	cout << file << endl;
    if (!dataFile) cout << "FILE NOT FOUND" << endl;
    dataFile >> nbVehicles;
    dataFile >> capacity;

    for (i = 0; i < nbNodes; i++) {
        dataFile >> n >> X[i] >> Y[i];
        dataFile >> quantity[i];
        dataFile >> startTW[i] >> endTW[i];
        dataFile >> serviceCost[i];
    }
    dataFile.close();

    for (i = 0; i < nbNodes; i++)
        for (j = 0; j < nbNodes; j++)
            if (i == j) {
                distance[i][j] = MyData::infinity;
                cost[i][j] = MyData::infinity;
            } else {
                x = X[i] - X[j];
                y = Y[i] - Y[j];
                distance[i][j] = double(int(10 * sqrt(double(x * x + y * y)))) / 10.0;
                cost[i][j] = distance[i][j] - 20;
            }


    delete[] X;
    delete[] Y;
}

void MyData::printEdges() {
    ofstream file("../edges.dat");
    if (file.is_open()) {
        for (int i = 0; i < nbNodes; i++)
            for (int j = 0; j < nbNodes; j++)
                if (i != j) file << i << " " << j << " " << cost[i][j] << " " << distance[i][j] << endl;
        file.close();
    } else
        cout << "Unable to open file";
}
