#include <iostream>
// #include <omp.h>
#include <vector>
#include <bitset>
#include <algorithm>
#include <queue>

using namespace std;

// TODO: fix this to be just subset finder with more optimization from this paper
// https://arxiv.org/pdf/1904.00051

namespace MVC
{

    class Graph
    {
    public:
        int n_vertices;
        vector<vector<int> > adj;
        vector<int> bestMVC;
        int bestCost = 1e6;

        Graph(int n) : bestMVC(n), adj(n), n_vertices(n) {};

        void addEdge(int u, int v)
        {
            adj[u].push_back(v);
            adj[v].push_back(u);
        }

        void calcMVC()
        {
            vector<int> picks;
            vector<bool> coverage(this->n_vertices, false), visited(this->n_vertices, false);

            pickZeroDegreeNode(coverage,picks);

            for(int i = 0; i < this->n_vertices ; i++) {
                recursiveMVC(picks, coverage, visited, i , -1);
            }
        }

        bool isFullCoverage(vector<bool> &coverage) {
            for(bool x : coverage) if(!x) return false;
            return true;
        }

        void pickZeroDegreeNode(vector<bool> &coverage,vector<int> &picks) {
            int i = 0;
            for(auto x : this->adj) {
                if(x.size() == 0) {
                    coverage[i] = 1;
                    picks.push_back(i);
                }
                i++;
            }
        }

        void recursiveMVC(vector<int> &picks, vector<bool> &coverage, vector<bool> &visited, int currVertex , int origin ) {

            if(picks.size() > bestCost) {

                // BEGIN : DEBUG
                cout<<"The result was bounded, Terminating this flow\n";
                // END : DEBUG

                return;
            }

            // BEGIN : DEBUG
            cout<<"currVertex "<<currVertex<<" parent "<<origin<<"\n";
            for(auto y : coverage) cout<<y<<" ";
            cout<<"\n";
            // END : DEBUG

            if (isFullCoverage(coverage)) {

                // BEGIN : DEBUG
                cout<<"Got full coverage\n";
                // END : DEBUG
                
                if (picks.size() < this->bestCost) {
                    this->bestMVC = picks;
                    this->bestCost = picks.size();
                }
                return;
            }
            
            if(visited[currVertex]) {

                // BEGIN : DEBUG
                cout<<"Hit deadend\n";
                // END : DEBUG

                return;
            }

            visited[currVertex] = true;

            vector<bool> temp_coverage = coverage;

            for(auto x : this->adj[currVertex]) {
            
                // Route 1 : don't pick currVertex

                // BEGIN : DEBUG
                cout<<"currVertex "<<currVertex<<" parent "<<origin<<"\n";
                cout<<"B2 : not picking "<<currVertex<<"\n";
                cout<<"Goto "<<x<<"\n";
                for(auto y : coverage) cout<<y<<" ";
                cout<<"\n";
                // END : DEBUG

                recursiveMVC(picks,coverage,visited,x,currVertex);
                coverage = temp_coverage;

                // Route 2 : pick currVertex

                // BEGIN : DEBUG
                cout<<"currVertex "<<currVertex<<" parent "<<origin<<"\n";
                cout<<"B1 : pick "<<currVertex<<"\n";
                cout<<"Goto "<<x<<"\n";
                // END : DEBUG

                picks.push_back(currVertex);
                coverage[currVertex] = true;
                for(auto neighbour : this->adj[currVertex]) coverage[neighbour] = true;
                
                // BEGIN : DEBUG
                for(auto y : coverage) cout<<y<<" ";
                cout<<"\n";
                // END : DEBUG
                recursiveMVC(picks,coverage,visited,x,currVertex);
                coverage = temp_coverage;
                picks.pop_back();

            }

            visited[currVertex] = false;
        }
    };
}