#include "espprc.h"

int main() {

    int nbCustomers[] = {5, 10, 15, 25, 50, 75, 100};

    for (int i = 0; i < sizeof(nbCustomers)/sizeof(*nbCustomers); i++) {
        string fileName = "../instances/r101-100.dat";
        MyData *data = new MyData(fileName, nbCustomers[i]);
        ESPPRC *espprc = new ESPPRC(data);
        espprc->solve();

        list<Route>::iterator iter;

        cout << endl << fileName << ", " << nbCustomers[i] << " Customers" << endl;

        for (iter = espprc->getRouteList()->begin(); iter != espprc->getRouteList()->end(); iter++) {
            cout << *iter;
        }

        cout << "Number of nondominated routes: " << espprc->getRouteList()->size() << endl;
    }

    return 0;
}