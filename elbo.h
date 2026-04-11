#pragma once
#include "type.h"
#include "graph.h"
#include "utils.h"
// #include <omp.h>

using namespace std;

namespace MDS {

    int lowerbound2(const Graph &g , problemInstance I) {
        set<int> undetermined_set = difference(g.v_set,I.X) , undominated_set = difference(g.v_set,sunion(N_Set(g,I.S),I.I));
        vector<int> undetermined_vec = set2vec(undetermined_set);
        vector<pair<int,int> > ranking(undetermined_vec.size()); // <-score,idx>
        #pragma omp parallel for schedule(dynamic)
        for(int i = 0; i < undetermined_vec.size() ; i++) {
           int c_x = C(g,I,undetermined_vec[i]).size();
           ranking[i] = make_pair(-1*c_x,undetermined_vec[i]);
        }
        sort(ranking.begin(),ranking.end());
        int l2 = 0;
        int cumcoverage = 0;
        for(auto v : ranking) {
            l2 += 1;
            cumcoverage += -1 * v.first;
            if(cumcoverage >= undominated_set.size()) break;
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