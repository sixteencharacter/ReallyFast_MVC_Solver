#include <iostream>
// #include <omp.h>
#include <vector>
#include <map>
#include <unordered_map> // Added missing header
#include <algorithm>
#include <cstdlib>

using namespace std;

namespace MVC
{

    class Graph
    {
    public:
        int n_vertices;
        vector<vector<int> > adj;
        vector<int> bestMVC;
        unordered_map<int,int> reverseHeuristicMap;
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
            
            // Start the parallel region
            #pragma omp parallel
            {
                // Ensure only one thread starts the initial recursive call
                #pragma omp single
                {
                    recursiveMVC(picks);
                }
            }
        }

        void printResult() {
            sort(this->bestMVC.begin(),this->bestMVC.end());
            int ref = 0;
            for(int i = 0 ; i < this->n_vertices ; i++) {
                if(ref < this->bestMVC.size() && i == this->bestMVC[ref]) {
                    cout << 1;
                    ref++;
                } else {
                    cout << 0;
                }
            }
        }

        int calcCoverage(vector<int> &picks) {
        //     cout<<"Coverage ";
        //    for(auto p : picks) cout<<this->reverseHeuristicMap[p]<<" ";
        //    cout<<"\n";
           unordered_map<int,int> mp;
           for(int p : picks) {
                mp[this->reverseHeuristicMap[p]] = 1;
                for(int n : this->adj[this->reverseHeuristicMap[p]]) {
                   mp[n] = 1;
                }
            }
            return mp.size();
        }

        void populateOrdering() {
            vector<pair<int,int> > nodedegs; 
            int i = 0;
            for(auto a : this->adj) nodedegs.push_back(make_pair((int)a.size(),i++));
            
            sort(nodedegs.begin(),nodedegs.end(),[](const pair<int,int> &a , const pair<int,int> &b) {
                if(a.first == b.first) return ((double)rand() / RAND_MAX) > 0.5;
                return b.first < a.first;
            });
            
            i = 0;
            for(pair<int,int> n : nodedegs) {this->reverseHeuristicMap[i] = n.second; i++;}
        }

        void recursiveMVC(vector<int> picks, int depth = 0) {
            
            int currentBest;

            #pragma omp atomic read
            currentBest = this->bestCost;

            if(picks.size() > currentBest) {
                return;
            }

            if(depth == this->n_vertices - 1) {

                if (calcCoverage(picks) == this->n_vertices) {
                    vector<int> ansIdx;
                    for(auto p : picks) ansIdx.push_back(this->reverseHeuristicMap[p]);
                    

                    #pragma omp critical
                    {
                        if (ansIdx.size() <= this->bestCost) {
                            this->bestMVC = vector<int>(ansIdx);
                            this->bestCost = ansIdx.size();
                        }
                    }
                }
                return;
            }

            vector<int> picks_with = picks;
            picks_with.push_back(depth);

            if (depth < 10) {

                #pragma omp task shared(reverseHeuristicMap,bestMVC,bestCost,n_vertices,adj) firstprivate(picks_with, depth)
                recursiveMVC(picks_with, depth + 1);
                
                #pragma omp task shared(reverseHeuristicMap,bestMVC,bestCost,n_vertices,adj) firstprivate(picks_with, depth)
                recursiveMVC(picks, depth + 1);

                #pragma omp taskwait

            } else {
                if(((double)rand() / RAND_MAX) > 0.5) {                
                    recursiveMVC(picks_with, depth+1);
                    recursiveMVC(picks, depth+1);
                } else {
                    recursiveMVC(picks, depth+1);
                    recursiveMVC(picks_with, depth+1);
                }
            }
        }
    };
}