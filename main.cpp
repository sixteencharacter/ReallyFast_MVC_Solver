#include <iostream>
#include <vector>
// #include <omp.h>
#include "graph.h"

using namespace std;
using namespace MVC;

// int max_threads = omp_get_max_threads();

int main(int args , char** kwargs) {
    int v, e;
    // printf("Running with %d threads",max_threads);
    cin>>v>>e;
    Graph g = Graph(v);
    for(int i = 0 ; i < e ; i++) {
        int f, t;
        cin>>f>>t;
        g.addEdge(f,t);
    }
    g.calcMVC();
    cout<<"Min cost: "<<g.bestCost<<"\nConfiguration: ";
    for(auto x : g.bestMVC) cout<<x<<" ";
    cout<<"\n";
    return 0;
}