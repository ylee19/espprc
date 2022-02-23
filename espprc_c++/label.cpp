#include "label.h"

MyData *Label::appl = NULL;

Label::Label() {
    int i;
    cost = 0.0;
    time = 0.0;
    examined = 0;		//is this label extended?
    attachedNode = 0;	//last vertex of the partial path
    unreachableNodes = 0;	//nodes which are not reachable
    succ = new short int[appl->getNbOfNodes()];
    for (i = 0; i < appl->getNbOfNodes(); i++)
        succ[i] = -1;
}

Label::Label(Label const &val) {
    int i;
    cost = val.cost;
    time = val.time;
    examined = val.examined;
    attachedNode = val.attachedNode;
    unreachableNodes = val.unreachableNodes;
    succ = new short int[appl->getNbOfNodes()];
    for (i = 0; i < appl->getNbOfNodes(); i++)
        succ[i] = val.succ[i];

    succNonElem = val.succNonElem;
}


Label::~Label() {
    delete[] succ;
}

int Label::extend(int j) {
    int i = attachedNode;

    if (j != 0) // extension to a customer
    {
        if (succ[j] != -1) return 0; // already visited or unreachable
        // check feasibility and update resources
        time = time + appl->getServiceCost(i) + appl->getDistance(i, j);
        if (time < appl->getStartWindow(j)) time = appl->getStartWindow(j);
        if (time > appl->getEndWindow(j)) return 0;
        
		// 새로운 label 생성됨
		// update cost and other parameters
        cost = cost + appl->getCost(i, j);
        examined = 0;		//새로운 label은 examine 되어야 함.
        attachedNode = j;	//마지막 node가 j로 바뀜.
        unreachableNodes = unreachableNodes + 1; // customer j became unreachable
        succ[j] = -2; //unreachable
        succ[i] = j;	//i 다음에 j가 온다. 
        for (i = 1; i < appl->getNbOfNodes(); i++)
            if (succ[i] == -1) //i 가 아직 방문 안되고 reachable인 경우, 얘가 still reachable인지 다시한번 check 한다.
                if ((time + appl->getServiceCost(j) + appl->getDistance(j, i) > appl->getEndWindow(i))) //i not reachable from j
                {
                    succ[i] = -2;
                    unreachableNodes = unreachableNodes + 1;
                }
    } 
	else // extension to the depot
    {
        succ[i] = 0;
        time = time + appl->getServiceCost(i) + appl->getDistance(i, 0);
        cost = cost + appl->getCost(i, 0);
        attachedNode = j;
        examined = 1;
    }

    return 1; //extension carried out
}

Label const &Label::operator=(Label const &L2) {
    if (this == &L2) return *this;
    cost = L2.cost;
    time = L2.time;
    examined = L2.examined;
    attachedNode = L2.attachedNode;
    unreachableNodes = L2.unreachableNodes;
    for (int i = 0; i < appl->getNbOfNodes(); i++)
        succ[i] = L2.succ[i];
    succNonElem = L2.succNonElem;
    return *this;
}



//return 0: time이나 cost 둘 중 하나 다를 때.
//return 1: time이랑 cost는 같은데, 지금까지 방문했던 node set이 다를 때.
//return 2: time이랑 cost 같고, 지금까지 방문한 node set이 동일.
int operator==(const Label &L1, const Label &L2) {
    if (fabs(L1.time - L2.time) > Label::appl->getMyZero()) return 0;
    if (fabs(L1.cost - L2.cost) > Label::appl->getMyZero()) return 0;

    // only when non elementary routes are not allowed
    if (L1.unreachableNodes != L2.unreachableNodes) return 1;
    for (int i = 0; i < L1.appl->getNbOfNodes(); i++)
        if (((L1.succ[i] == -1) && (L2.succ[i] != -1)) || ((L1.succ[i] != -1) && (L2.succ[i] == -1))) return 1;

    return 2;
}

//시간이나 cost 둘 중 하나라도 다르면 true
//둘다 같으면 false
bool operator!=(const Label &L1, const Label &L2) {
    if (fabs(L1.time - L2.time) > Label::appl->getMyZero()) return true;
    if (fabs(L1.cost - L2.cost) > Label::appl->getMyZero()) return true;
    return false;
}

// lexigraphic order on time and cost 순으로 L1이 L2보다 좋으면 return 1
// L1이 L2를 dominate하면 return 2. 
// 즉, 시간도 적고 cost도 작고, 방문 node 수도 더 적고,
// L2에서 방문 가능한 애는 L1에서도 방문 가능한 경우 return 2

//if L1 < L2 and L1 dominates L2, return 2
int operator<(const Label &L1, const Label &L2) {
	//L1이 시간 더 많이 소모했으면 return 0
	if (L1.time > L2.time + Label::appl->getMyZero()) return 0;

	//동일한 시간에 reduced cost가 더 크면 return 0
	if (L1.time > L2.time - Label::appl->getMyZero()) {
        if (L1.cost > L2.cost - Label::appl->getMyZero())
            return 0;
    }
	// 여기부터는 L1의 시간이 더 작음. 즉 lexicographically better!
    // Now check the dominance
    if (L1.cost > L2.cost + Label::appl->getMyZero()) return 1;

    // only when non elementary routes are not allowed
    if (L1.unreachableNodes > L2.unreachableNodes) return 1;
    for (int i = 0; i < L1.appl->getNbOfNodes(); i++)
        if ((L1.succ[i] != -1) && (L2.succ[i] == -1)) return 1;
    return 2;
}


bool operator<=(const Label &L1, const Label &L2) {
    if (L1.time < L2.time - Label::appl->getMyZero()) return true;
    if (L1.time > L2.time + Label::appl->getMyZero()) return false;
    
	//time이 같은 경우에만 cost를 비교
	if (L1.cost < L2.cost - Label::appl->getMyZero()) return true;
    if (L1.cost > L2.cost + Label::appl->getMyZero()) return false;
    return true;
}


ostream &operator<<(ostream &flot, Label &label) {
    flot << "  Time    = " << label.getTime() << endl;
    flot << "  Cost    = " << label.getCost() << endl;

    return flot;
}

// transform a label into a route
Route Label::findRoute() {
    int i;
    Route R(appl);

    i = succ[0];
    while (i != 0) {
        R.push_back_customer(i);
        i = succ[i];
    }
    return R;
}
