#include <iostream>
// #include <omp.h>
#include <vector>
#include <map>
#include <algorithm>
#include <cstdlib>

using namespace std;

// TODO: fix this to be just subset finder with more optimization

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
            recursiveMVC(picks);
        }

        void printResult() {
            sort(this->bestMVC.begin(),this->bestMVC.end());
            int ref = 0;
            for(int i = 0 ; i < this->n_vertices ; i++) {
                if(i == this->bestMVC[ref]) {
                    cout<<1;
                    ref++;
                } else {
                    cout<<0;
                }
            }
        }

        int calcCoverage(vector<int> &picks) {
           unordered_map<int,int> mp;
           for(int p : picks) {
                mp[this->reverseHeuristicMap[p]] = 1;
                for(int n : this->adj[this->reverseHeuristicMap[p]]) {
                   mp[n] = 1;
                }
            }

            // // Debug
            // cout<<"\nPicks: ";
            // for(auto p : picks) cout<<p<<" ";
            // cout<<"\n";

            // cout<<"MP Size : "<<mp.size()<<"\n";

            // // Debug

            return mp.size();
        }

        void populateOrdering() {
            vector<pair<int,int> > nodedegs; // <deg,node>
            int i = 0;
            for(auto a : this->adj) nodedegs.push_back(make_pair((int)a.size(),i++));
            sort(nodedegs.begin(),nodedegs.end(),[](const pair<int,int> &a , const pair<int,int> &b) {
                if(a.first == b.first) return ((rand() / RAND_MAX) > 0.5)? true : false;
                return b.first < a.first;
            });
            i = 0;
            for(pair<int,int> n : nodedegs) {this->reverseHeuristicMap[i] = n.second; i++;}

            // DEBUG : START
            // for(auto x : nodedegs) cout<<x.second<<"("<<x.first<<")"<<" ";
            // for(auto x = this->reverseHeuristicMap.begin() ; x != this->reverseHeuristicMap.end() ; x++ ) {
            //     cout<<x->first<<"->"<<x->second<<"\n";
            // }
            // cout<<"\n";
            // DEBUG : END
        }

        void recursiveMVC(vector<int> picks,int depth = 0) {
            
            if(picks.size() > bestCost) {

                // BEGIN : DEBUG
                // cout<<"The result was bounded, Terminating this flow\n";
                // END : DEBUG

                return;
            }

            if(depth == this->n_vertices - 1) {

                if (calcCoverage(picks) == this->n_vertices) {
    
                    // BEGIN : DEBUG
                    // cout<<"Got full coverage\n";
                    // END : DEBUG

                    vector<int> ansIdx;
                    for(auto p : picks) ansIdx.push_back(this->reverseHeuristicMap[p]);

                    // // DEBUG
                    // cout<<"ansIdx: ";
                    // for(auto x : ansIdx) cout<<x<<" ";
                    // cout<<"\n";
                    // // DEBUG
                    
                    if (picks.size() <= this->bestCost) {
                        this->bestMVC = vector<int>(ansIdx);
                        this->bestCost = ansIdx.size();
                    }
                }

                return;

            }

            if((rand() / RAND_MAX) > 0.5) {                
                // case 2 : selecting this vertex
                picks.push_back(depth);
                recursiveMVC(picks,depth+1);
                picks.pop_back();
    
                // case 1 : not selecting this vertex
                recursiveMVC(picks,depth+1);
            } else {
                // case 1 : not selecting this vertex
                recursiveMVC(picks,depth+1);

                // case 2 : selecting this vertex
                picks.push_back(depth);
                recursiveMVC(picks,depth+1);
                picks.pop_back();
            }
            
        }

    };
}