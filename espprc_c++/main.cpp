#include "espprc.h"

int main() {

    int nbCustomers[] = {5, 10, 15, 25, 50, 75, 100};

	double **z_dual, *y_dual;
	int **sc, **st;
	int CAPA = 100;
	z_dual = new double *[nbCustomers[0] + 1];
	sc = new int *[nbCustomers[0] + 1];
	st = new int *[nbCustomers[0] + 1];
	y_dual = new double[nbCustomers[0] + 1]{ 0 };
	for (int i = 0; i < nbCustomers[0]+1; i++) {
		z_dual[i] = new double[nbCustomers[0] + 1]{ 0 };
		sc[i] = new int[nbCustomers[0] + 1]{ 0 };
		st[i] = new int[nbCustomers[0] + 1]{ 0 };
	}
	for (int i = 0; i < nbCustomers[0] + 1; i++) {
		for (int j = 0; j < nbCustomers[0] + 1; j++) {
			sc[i][j] = -i - j;
			st[i][j] = 1;
		}
	}
	st[2][4] = 1000;
	MyData *data2 = new MyData(z_dual, y_dual, sc, st, CAPA, 5);
	ESPPRC *espprc2 = new ESPPRC(data2);
	espprc2->solve();
	list<Route>::iterator iter;
	list<Route> rl;
	cout << nbCustomers[0] << " Customers" << endl;
	rl =*espprc2->getRouteList();
	for (iter = rl.begin(); iter != rl.end(); iter++) {
		cout << *iter;
	}
	//for (iter = espprc2->getRouteList()->begin(); iter != espprc2->getRouteList()->end(); iter++) {
//		cout << *iter;
//	}

	cout << "Number of nondominated routes: " << espprc2->getRouteList()->size() << endl;
	
	rl = *espprc2->getRouteList();
	espprc2->processColinfo(rl);

    /*for (int i = 0; i < sizeof(nbCustomers)/sizeof(*nbCustomers); i++) {
        string fileName = "./instances/r211-100.dat";
        MyData *data = new MyData(fileName, nbCustomers[i]);
        ESPPRC *espprc = new ESPPRC(data);
        espprc->solve();

        list<Route>::iterator iter;

        cout << endl << fileName << ", " << nbCustomers[i] << " Customers" << endl;

        for (iter = espprc->getRouteList()->begin(); iter != espprc->getRouteList()->end(); iter++) {
            cout << *iter;
        }

        cout << "Number of nondominated routes: " << espprc->getRouteList()->size() << endl;
    }*/

    return 0;
}