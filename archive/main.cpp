#include <iostream>
#include <vector>
// #include <omp.h>
#include "graph.h"
#include <cstdlib>
#include <ctime>

using namespace std;
using namespace MVC;

// int max_threads = omp_get_max_threads();

int main(int args , char** kwargs) {
    srand(time(0));
    int v, e;
    // printf("Running with %d threads",max_threads);
    cin>>v>>e;
    Graph g = Graph(v);
    for(int i = 0 ; i < e ; i++) {
        int f, t;
        cin>>f>>t;
        g.addEdge(f,t);
    }
    g.populateOrdering();
    g.calcMVC();
    g.printResult();
    return 0;
}