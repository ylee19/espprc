#include "espprc.h"

ESPPRC::ESPPRC(MyData *ap) {
    list<int>::iterator iter;
    appl = ap;
    nbNodes = appl->getNbOfNodes();
    distance = appl->getDistances();
    succ = new list<int>[nbNodes];
    selectGraph();
	
	//�� node ���� label list �ϳ��� ����
    labelsNode = new ListLabel[nbNodes];
    Label::setData(appl);
}

ESPPRC::~ESPPRC() {
    delete[] labelsNode;
    delete[] succ;
}

int ESPPRC::selectGraph()
{
    bool** arc;
    int i,j,k;
    list<int>* succAppl=appl->getSuccessors();
    list<int>::iterator iter;

    arc=new bool*[nbNodes];
    for (i=0;i<nbNodes;i++)
        arc[i]=new bool[nbNodes];
    for (i=0;i<nbNodes;i++)
        for (j=0;j<nbNodes;j++)
            arc[i][j]=true;


    //generate arc set
    for (i=0;i<nbNodes;i++)
        for (iter=succAppl[i].begin();iter!=succAppl[i].end();iter++)
            if (arc[i][*iter])
                succ[i].push_back(*iter);

    for (i=0;i<nbNodes;i++)
        delete[] arc[i];
    delete[] arc;
    return 0;
}

// To-be!
// void Price::processColinfo_labeling()
// this->espprc->rlist
void ESPPRC::processColinfo(list<Route> rlist) {
	//Members of price class
	vector< vector < pair< int, int> > > seqInfo;
	int numCol = rlist.size();
	this->routeList;
	
	int i = 0;
	int prev = -1;
	list<Route>::iterator it_rt; //Iterator for routes
	list<int>::iterator it;	//iterator for items in each route
	for (it_rt = rlist.begin(); it_rt != rlist.end(); it_rt++) {
		seqInfo.push_back(vector<pair<int, int> >());
		for (it = it_rt->begin(); it != it_rt->end(); it++) {
			if (prev == -1) {
				prev = *it;
			}
			else {
				seqInfo[i].push_back(make_pair(prev, *it));
				prev = *it;
			}
		}
		i++;
		prev = -1;
	}
	cout << "processColinfo" << endl;

	for (int nnc = 0; nnc < numCol; nnc++) {
		cout << nnc << " : ";
		for (vector<pair<int, int> >::iterator iter = seqInfo[nnc].begin(); iter != seqInfo[nnc].end(); iter++) {
			cout << "(" << iter->first << " - " << iter->second << ")\t";
		}
		cout << endl;
	}
}

// extend labels associated with the current vertices
// update the list of vertices that should be considered next (pendingVertices)
int ESPPRC::extendLabels(bool *pendingVertices, int current) {
    int res;
    Label lb;
    int prolongate;
    list<Label>::iterator iterLabel;
    list<int>::iterator iterInt;

    for (iterLabel = labelsNode[current].begin(); iterLabel != labelsNode[current].end(); ++iterLabel) {
        if ((*iterLabel).getExamined() == 0) {	//���� �� label�� extend �õ��� ���غ�.
            //extend the label to every possible successor
            for (iterInt = succ[current].begin(); iterInt != succ[current].end(); iterInt++) {
                lb = *iterLabel; // create a new label by copy
				//���� label�� lb�� ���ο� item�� iterInt �� �ٿ�����.
                prolongate = lb.extend(*iterInt); //try extension
                if (prolongate) //if extension succeeded
                {
                    if (*iterInt != 0)//extension to a customer
                    {
                        res = labelsNode[*iterInt].efficientInsertion(lb);//insert (and possibly dominates) or be dominated
                        if (res) // the label was inserted
                            pendingVertices[*iterInt] = true;
                    } 
					else //extension to the depot
                    {
                        if (lb.getCost() < -appl->getMyZero())
                            labelsNode[*iterInt].efficientInsertion(lb);
                    }
                }
            }
            iterLabel->setExamined();
        }

    }
    return 0;
}


int ESPPRC::solve() {
    int i;
    bool *pendingVertices; 	//list of vertices that should be considered next
    Route R(appl);	// 0->0 route ����. cost=0, time=0, nbClients =0
    list<Label>::iterator iter;
    list<int>::iterator itI;
    Label LabelInit;	//�ʱ� label. ���� ����.

    labelsNode[0].efficientInsertion(LabelInit); //���� node�� intial label�� ���� ����.
	
	pendingVertices = new bool[nbNodes];
    pendingVertices[0] = true;	//0���� �����ؼ� �ٸ� item��� extend
    for (i = 1; i < nbNodes; i++)
        pendingVertices[i] = false;	//������ vertex���� ���� consider���� ����.

    for (i = 0; i < nbNodes; i++)
        if (pendingVertices[i]) {
            pendingVertices[i] = false;
            extendLabels(pendingVertices, i);	//item i�� extend
        }

    routeList.clear();

    for (iter = labelsNode[0].begin(); iter != labelsNode[0].end(); iter++)
        if (iter->getTime() != 0) //avoid the null label
            if (iter->getCost() < -appl->getMyZero())
                routeList.push_back((*iter).findRoute());


    delete[] pendingVertices;
    return 0;
}
