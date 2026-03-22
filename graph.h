#pragma once
#include <iostream>
#include <vector>
#include <set>

using namespace std;

namespace MDS
{

    class Graph
    {
    public:
        int n_vertices;
        vector<vector<int> > adj;
        set<int> v_set;

        Graph(int n) : adj(n), n_vertices(n) {
            for(int i = 0 ; i < n ; i++) v_set.insert(i);
        };

        void addEdge(int u, int v)
        {
            adj[u].push_back(v);
            adj[v].push_back(u);
        }

    };
}