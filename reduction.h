#pragma once
#include "type.h"
#include "graph.h"
#include "utils.h"
#include <omp.h>

using namespace std;

namespace MDS {

    void RR1(const Graph &g , problemInstance &I , int &change) {
        // outer loop
        vector<int> iterations = set2vec(difference(g.v_set,sunion(I.X,I.S)));
        #pragma omp parallel for schedule(dynamic)
        for(int u : iterations) {
            // RR1
            set<int> n_u = N(g,I,u) , single_node = {u};
            for(int v : g.v_set) {
                set<int> d_v = D(g,I,v);
                if(find(n_u.begin(),n_u.end(),v) != n_u.end() && d_v.size() == 1 && *d_v.begin() == u) {
                    #pragma omp critical 
                    {
                        if(find(I.S.begin(),I.S.end(),u) == I.S.end() && find(I.X.begin(),I.X.end(),u) == I.X.end()) {
                            I.S.insert(u);
                            change++;
                        }
                    }
                    break;
                }
            }
        }
    }

    void RR2(const Graph &g , problemInstance &I , int &change) {
        vector<int> iterations = set2vec(difference(g.v_set,sunion(I.X,I.S)));
        #pragma omp parallel for schedule(dynamic)
        for(int u : iterations) {
            set<int> c_u = C(g,I,u); 
            //RR2
            for(int v : g.v_set) {
                if(u == v) continue;
                set<int> n_v = N(g,I,v);
                if(issubset(c_u,n_v)) {
                    # pragma omp critical 
                    {
                        if(find(I.X.begin(),I.X.end(),u) == I.X.end()  && find(I.S.begin(),I.S.end(),u) == I.S.end()) {
                            I.X.insert(u);
                            change++;
                        }
                    }
                    break;
                }
            }
        }
    }

    void RR3(const Graph &g , problemInstance &I , int &change) {
        vector<int> iterations = set2vec(difference(g.v_set,sunion(N_Set(g,I.S),I.I)));
        #pragma omp parallel for schedule(dynamic)
        for(int u : iterations) {
            set<int> d_u = D(g,I,u);
            for(int v : g.v_set) {
                if(u == v) continue;
                set<int> single_node = {v} , n_v = N(g,I,v);
                if(issubset(d_u,n_v)) {
                    # pragma omp critical 
                    {
                        if(find(I.I.begin(),I.I.end(),v) == I.I.end()) {
                            I.I.insert(v);
                            change++;
                        }
                    }
                }
            }
        }
    }

    problemInstance reduceProblem(const Graph &g,problemInstance I) {
        int change = 1;
        while(change > 0) {
            change = 0;
            RR1(g,I,change);
            RR2(g,I,change);
            RR3(g,I,change);
        }
        return I;
    }

}