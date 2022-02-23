#pragma once

#include <vector>

using namespace std;

#include "my_data.h"
#include "route.h"


// Labels are used in the dynamic programming algorithm when solving the pricing problem.
// A single initial label will be generated to start the dynamic programming recursion, 
// when all other labels are obtained using the extend() method. 
// All the parameters of the labels are automatically updated with the method.
// Labels can be transformed to routes using the findRoute() method.
// Label comparison is performed with several complicated operators:

// Operator == : return 1 if two labels are equal for the cost and for the time; 
//				if in addition they are strictly identical it returns 2

// Operator != : return true if the two labels are different for either cost or one of the two resources consumption
// Operator < : return 1 if the first label is better than the second according to the lexicographic order time – cost; 
//				if in addition the first label dominates the second it returns 2
// Operator <= : return true if first label is better than or equal to the second according to the lexicographic order time – cost

class Label {
    friend int operator==(const Label &L1, const Label &L2);

    friend int operator<(const Label &L1, const Label &L2);

    friend bool operator<=(const Label &R1, const Label &L2);

    friend bool operator!=(const Label &R1, const Label &L2);

    friend ostream &operator<<(ostream &flot, Label &label);

public :
    Label();

    Label(Label const &val);

    ~Label();

    Label const &operator=(Label const &L2);

    // create the route encoded by the label
    Route findRoute();

    double getCost() const { return cost; }

    double getTime() const { return time; }

    // boolean that indicates if the label has been extended
    int getExamined() const { return examined; }

    // extend the label to vertex j
    int extend(int j);

    // change the status of the label to examined (has been extended)
    void setExamined() { examined = 1; }

    static MyData *getData() { return appl; }

    static void setData(MyData *ap) {
        appl = ap;
    }

private:
    // last vertex of the partial path
    int attachedNode;
    double cost;
    double time;


    // boolean value to indicate if the label has been extended or not
    int examined;
    static MyData *appl;


    // number of unreachable nodes
    int unreachableNodes;
	// partial path and unreachable nodes; only managed when non elementary routes are not allowed
    
	short int *succ;
    // sequences of visited customers; only managed when non elementary routes are allowed
    // definition of succ[i]:
    // if i is in the path and not at the end: succ[i] is the successor node
    // if i is unreachable, succ{i]=-2
    // succ[attachedNode]=-2 (visited, and so unreachable, but no successor)
    // if i is not visited and is reachable, succ[i]=-1
    vector<int> succNonElem;
};



