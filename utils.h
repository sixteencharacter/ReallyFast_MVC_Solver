#pragma once
#include <set>
#include <tuple>
#include <algorithm>
#include <iterator>
#include "graph.h"
#include "type.h"

using namespace std;

namespace MDS {

    set<int> difference(const set<int> &a ,const set<int>&b , set<int> *container = NULL) {
        set<int> ret;
        if(container == NULL) set_difference(a.begin(),a.end(),b.begin(),b.end(),inserter(ret,ret.begin()));
        else set_difference(a.begin(),a.end(),b.begin(),b.end(),inserter(*container,container->begin()));
        return ret;
    }
    
    set<int> sunion(const set<int> &a ,const set<int>&b , set<int> *container = NULL) {
        set<int> ret;
        if(container == NULL) set_union(a.begin(),a.end(),b.begin(),b.end(),inserter(ret,ret.begin()));
        else set_union(a.begin(),a.end(),b.begin(),b.end(),inserter(*container,container->begin()));
        return ret;
    }

    bool issubset(const set<int> &b , const set<int> &a) {
        return includes(a.begin(),a.end(),b.begin(),b.end());
    }
    
    set<int> N(const Graph &g , problemInstance &I,int u) {
        set<int> ret;
        for(int n : g.adj[u]) ret.insert(n);
        ret.insert(u);
        return ret;
    }

    set<int> N_Set(const Graph &g , set<int> S) {
        set<int> ret;
        for(int s : S) {
            for(int v : g.adj[s]) ret.insert(v);
            ret.insert(s);
        }
        return ret;
    }

    set<int> D(const Graph &g , problemInstance &I,int u) {
        set<int> n_u = N(g,I,u) , ret;
        difference(n_u,I.X,&ret);
        return ret;
    }

    set<int> C(const Graph &g , problemInstance &I,int u) {
        set<int> n_u = N(g,I,u) , N_s = N_Set(g,I.S) , ret;
        difference(n_u,sunion(N_s,I.I),&ret);
        return ret;
    }

    int sampleV(const Graph &g , problemInstance &I) {
        set<int> undominated = difference(g.v_set,sunion(N_Set(g,I.S),I.I));
        vector<tuple<int,int,int> > ranking; // |D_v| , sum_(d_v){|C_u|} , index
        for(int v : undominated) {
            set<int> d_v = D(g,I,v);
            int args1 = d_v.size();
            int args2 = 0;
            for(int u : d_v) args2 -= C(g,I,u).size();
            ranking.push_back(make_tuple(args1,args2,v));
        }
        sort(ranking.begin(),ranking.end());
        return get<2>(ranking[0]);
    }

    vector<pair<int,int> > sample_u_dv(const Graph &g , problemInstance &I, int v) {
        set<int> its = D(g,I,v);
        vector<pair<int,int> > ranking; // -|Cu| , |idx|
        for(auto u : its) {
            ranking.push_back(make_pair(-1*C(g,I,u).size(),u));
        }
        sort(ranking.begin(),ranking.end());
        return ranking;
    }
}