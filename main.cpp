#include <iostream>
#include <vector>
#include <omp.h>

using namespace std;

int max_threads = omp_get_max_threads();

class Graph {
    public :
        int n_vertices;
        vector<vector<int>> adj;
        vector<pair<int,int>> edges;
        Graph(int n) : adj(n) , n_vertices(n) {};
        void addEdge(int u , int v) {
            adj[u].push_back(v);
            adj[v].push_back(u);
            edges.push_back(make_pair(u,v));
        }
};

int main(int args , char** kwargs) {
    int n , v, e;
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