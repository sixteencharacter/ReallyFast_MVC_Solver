#pragma once
#include <set>
#include <algorithm>
#include "graph.h"
#include "reduction.h"
#include "utils.h"
#include "elbo.h"
#include "type.h"
#include <iostream>

using namespace std;

namespace MDS {

    set<int> bibSearch(const Graph g,problemInstance I,set<int> DStar) {
        problemInstance IPrime = reduceProblem(g,I);
        set<int> Ns = N_Set(g,I.S);
        if(equal(Ns.begin(),Ns.end(),g.v_set.begin(),g.v_set.end())) {
            if(I.S.size() < DStar.size()) {
                DStar = I.S;
            }
            return DStar;
        }
        if((int)DStar.size() <= elbo(g,I)) {
            return DStar;
        }
        int v = sampleV(g,I);
        for(auto pa : sample_u_dv(g,I,v)) { // score , u
            problemInstance Iu = I;
            Iu.S.insert(pa.second);
            set<int> D = bibSearch(g,Iu,DStar);
            if(D.size() < DStar.size()) {
                DStar = D;
            }
            I.X.insert(pa.second);
        }
        return DStar;
    }

}