#pragma once
#include "type.h"
#include "graph.h"
#include "utils.h"

using namespace std;

namespace MDS {

    int lowerbound2(const Graph &g , problemInstance I) {
        set<int> undetermined_set = difference(g.v_set,I.X) , undominated_set = difference(g.v_set,sunion(N_Set(g,I.S),I.I));
        int l2 = 0;
        for(int v : undetermined_set) {
            if(C(g,I,v).size() >= undominated_set.size()) l2 += 1;
        }
        return I.S.size() + l2;
    }

    int lowerbound1(const Graph &g , problemInstance I) {
        set<int> undominated_set = difference(g.v_set,sunion(N_Set(g,I.S),I.I));
        vector<pair<int,int> > ranking; // <score,vertexId>
        for(int u : undominated_set) ranking.push_back(make_pair(D(g,I,u).size(),u));
        sort(ranking.begin(),ranking.end());
        vector<bool> visited(g.n_vertices,false);
        int l1 = 0;
        for(auto r : ranking) {
            set<int> v_dominant_set = D(g,I,r.second);
            bool isDisjoint = true;
            for(int vds: v_dominant_set) {
                if(visited[vds]) {
                    isDisjoint = false;
                    break;
                }
            }
            if(isDisjoint) {
                l1++;
                for(int vds : v_dominant_set) visited[vds] = true;
            }
        }
        
        return I.S.size() + l1;
    }

    int elbo(const Graph &g , problemInstance I) {
        return max(lowerbound2(g,I),lowerbound1(g,I));
    }


}