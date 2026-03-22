#pragma once
#include <iostream>
#include <vector>
#include "graph.h"
#include "solver.h"
#include "type.h"
#include "utils.h"
#include <cstdlib>
#include <ctime>
#include <omp.h>

using namespace std;
using namespace MDS;

int mx_thread = omp_get_max_threads();

int main(int args , char** kwargs) {
    omp_set_num_threads(mx_thread);
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
    problemInstance I;
    set<int> DStar = g.v_set;
    vector<int> result = set2vec(bibSearch(g,I,DStar));
    int i = 0;
    vector<bool> ans(g.n_vertices,false);
    for(int r : result) ans[r] = true;
    for(bool a : ans) cout<<a;
    return 0;
}