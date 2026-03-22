#include <iostream>
#include <vector>
#include <omp.h>

using namespace std;

int main(int args , char** kwargs) {
    int n , v, e;
    int max_threads = omp_get_max_threads();
    printf("Running with %d threads",max_threads);
    cin>>n>>v>>e;
    vector<int> adj[n];
    for(int i = 0 ; i < e ; i++) {
        int f, t;
        cin>>f>>t;
        adj[f].push_back(t);
        adj[t].push_back(f);
    }
    return 0;
}