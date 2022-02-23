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
        
		// ���ο� label ������
		// update cost and other parameters
        cost = cost + appl->getCost(i, j);
        examined = 0;		//���ο� label�� examine �Ǿ�� ��.
        attachedNode = j;	//������ node�� j�� �ٲ�.
        unreachableNodes = unreachableNodes + 1; // customer j became unreachable
        succ[j] = -2; //unreachable
        succ[i] = j;	//i ������ j�� �´�. 
        for (i = 1; i < appl->getNbOfNodes(); i++)
            if (succ[i] == -1) //i �� ���� �湮 �ȵǰ� reachable�� ���, �갡 still reachable���� �ٽ��ѹ� check �Ѵ�.
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



//return 0: time�̳� cost �� �� �ϳ� �ٸ� ��.
//return 1: time�̶� cost�� ������, ���ݱ��� �湮�ߴ� node set�� �ٸ� ��.
//return 2: time�̶� cost ����, ���ݱ��� �湮�� node set�� ����.
int operator==(const Label &L1, const Label &L2) {
    if (fabs(L1.time - L2.time) > Label::appl->getMyZero()) return 0;
    if (fabs(L1.cost - L2.cost) > Label::appl->getMyZero()) return 0;

    // only when non elementary routes are not allowed
    if (L1.unreachableNodes != L2.unreachableNodes) return 1;
    for (int i = 0; i < L1.appl->getNbOfNodes(); i++)
        if (((L1.succ[i] == -1) && (L2.succ[i] != -1)) || ((L1.succ[i] != -1) && (L2.succ[i] == -1))) return 1;

    return 2;
}

//�ð��̳� cost �� �� �ϳ��� �ٸ��� true
//�Ѵ� ������ false
bool operator!=(const Label &L1, const Label &L2) {
    if (fabs(L1.time - L2.time) > Label::appl->getMyZero()) return true;
    if (fabs(L1.cost - L2.cost) > Label::appl->getMyZero()) return true;
    return false;
}

// lexigraphic order on time and cost ������ L1�� L2���� ������ return 1
// L1�� L2�� dominate�ϸ� return 2. 
// ��, �ð��� ���� cost�� �۰�, �湮 node ���� �� ����,
// L2���� �湮 ������ �ִ� L1������ �湮 ������ ��� return 2

//if L1 < L2 and L1 dominates L2, return 2
int operator<(const Label &L1, const Label &L2) {
	//L1�� �ð� �� ���� �Ҹ������� return 0
	if (L1.time > L2.time + Label::appl->getMyZero()) return 0;

	//������ �ð��� reduced cost�� �� ũ�� return 0
	if (L1.time > L2.time - Label::appl->getMyZero()) {
        if (L1.cost > L2.cost - Label::appl->getMyZero())
            return 0;
    }
	// ������ʹ� L1�� �ð��� �� ����. �� lexicographically better!
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
    
	//time�� ���� ��쿡�� cost�� ��
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
