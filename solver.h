#pragma once
#include <set>
#include <algorithm>
#include "graph.h"
#include "reduction.h"
#include "util.h"
#include "elbo.h"
#include "type.h"
#include <iostream>

using namespace std;

namespace MDS {

    set<int> bibSearch(const Graph g,problemInstance I,set<int> DStar) {
        cout<<"Hit bibsearch\n";
        problemInstance IPrime = reduceProblem(g,I);
        cout<<"reduction\n";
        cout<<"---";
        cout<<"\nI: ";
        for(auto x : IPrime.I) cout<<x<<" ";
        cout<<"\n";
        cout<<"\nS: ";
        for(auto x : IPrime.S) cout<<x<<" ";
        cout<<"\n";
        cout<<"\nX: ";
        for(auto x : IPrime.X) cout<<x<<" ";
        cout<<"\n";
        cout<<"---";
        set<int> Ns = N_Set(g,I.S);
        if(equal(Ns.begin(),Ns.end(),g.v_set.begin(),g.v_set.end())) {
            cout<<"T1\n";
            if(I.S.size() < DStar.size()) {
                DStar = I.S;
            }
            return DStar;
        }
        if((int)DStar.size() <= elbo(g,I)) {
            return DStar;
        }
        int v = sampleV(g,I);
        cout<<"v: "<<v;
        for(auto pa : sample_u_dv(g,I,v)) { // score , u
            cout<<"T3\n";
            problemInstance Iu = I;
            Iu.S.insert(pa.second);
            set<int> D = bibSearch(g,Iu,DStar);
            cout<<"\n D's Size: "<<D.size()<<" D*'s Size: "<<DStar.size()<<"\n";
            if(D.size() < DStar.size()) {
                DStar = D;
            }
            I.X.insert(pa.second);
        }
        return DStar;
    }

}