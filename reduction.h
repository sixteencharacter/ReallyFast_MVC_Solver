#pragma once
#include "type.h"
#include "graph.h"
#include "utils.h"

using namespace std;

namespace MDS {

    void RR1(const Graph &g, problemInstance &I, int &change) {
        // 1. Get the set of undetermined vertices (potential dominators)
        vector<int> undetermined = set2vec(difference(g.v_set, sunion(I.X, I.S)));
        
        // Pre-calculate dominated set for efficiency
        set<int> Ns = N_Set(g, I.S);

        for(int u : undetermined) {
            // 2. RR1: If an UNDOMINATED vertex v has only one dominator u, add u to S
            // Check only the closed neighborhood of u
            for(int v : N(g, I, u)) { 
                // Only trigger if v is undominated: v is not in N[S] AND not in I
                if (Ns.find(v) == Ns.end() && I.I.find(v) == I.I.end()) {
                    set<int> d_v = D(g, I, v);
                    if(d_v.size() == 1 && *d_v.begin() == u) {
                        I.S.insert(u);
                        change++;
                        // u is now selected; update Ns and stop checking for this u
                        set<int> n_u = N(g, I, u);
                        Ns.insert(n_u.begin(), n_u.end());
                        break; 
                    }
                }
            }
        }
    }

    void RR2(const Graph &g, problemInstance &I, int &change) {
        // 1. Iterate over undetermined vertices u
        vector<int> undetermined = set2vec(difference(g.v_set, sunion(I.X, I.S)));
        
        for (int u : undetermined) {
            set<int> c_u = C(g, I, u);
            
            // If u covers nothing new, it is redundant
            if (c_u.empty()) {
                I.X.insert(u);
                change++;
                continue;
            }

            // 2. Optimization: Pick one vertex w0 that u covers
            int w0 = *c_u.begin();
            
            // 3. Only check v that can dominate w0 AND are not excluded
            for (int v : N(g, I, w0)) { // v is in N[w0]
                if (u == v || I.X.count(v)) continue; // v cannot be in X

                if (issubset(c_u, N(g, I, v))) { // N[v]
                    I.X.insert(u);
                    change++;
                    break; 
                }
            }
        }
    }

    void RR3(const Graph &g, problemInstance &I, int &change) {
        // 1. Get the set of undominated vertices
        set<int> undominated = difference(g.v_set, sunion(N_Set(g, I.S), I.I));
        
        for (int u : undominated) {
            set<int> d_u = D(g, I, u);
            if (d_u.empty()) continue; // u has no dominators, handled by branching/feasibility

            // 2. Efficiency: only check v that are neighbors of a dominator of u
            int w0 = *d_u.begin();
            for (int v : N(g, I, w0)) { // N[w0]
                // 3. v must be a distinct undominated vertex
                if (u == v || undominated.find(v) == undominated.end()) continue;

                if (issubset(d_u, N(g, I, v))) { // D(u) subset of N[v]
                    I.I.insert(v);
                    change++;
                    // Update undominated set to reflect that v is now ignored
                    undominated.erase(v);
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