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
        set<int> Ns = N_Set(g,IPrime.S);
        if(issubset(difference(g.v_set,IPrime.I),Ns)) {
            if(IPrime.S.size() < DStar.size()) {
                DStar = IPrime.S;
            }
            return DStar;
        }
        if((int)DStar.size() <= elbo(g,IPrime)) {
            return DStar;
        }
        int v = sampleV(g,IPrime);
        for(auto pa : sample_u_dv(g,IPrime,v)) { // score , u
            problemInstance Iu = IPrime;
            Iu.S.insert(pa.second);
            set<int> D = bibSearch(g,Iu,DStar);
            if(D.size() < DStar.size()) {
                DStar = D;
            }
            IPrime.X.insert(pa.second);
        }
        return DStar;
    }

}