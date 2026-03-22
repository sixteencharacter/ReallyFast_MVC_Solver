#include <iostream>
#include <vector>
#include <omp.h>
#include <stack>

using namespace std;

typedef pair<int,bool> pbc;

pbc checkCoverage(vector<vector<int>> &v, int decisionInt) {
    int n = v.size();
    vector<bool> visited(n,false);
    int unvisitedCount = n;
    stack<int> st;
    for(int i = 0 ; i < n ; i++) {
        if((decisionInt & (1 >> i)) == 1 >> i) {
            st.push(i);
            while(!st.empty()) {
                cout<<st.top()<<"\n";
                int idx = st.top(); st.pop();
                if(!visited[idx]) { // it's 1 : select
                    unvisitedCount -= 1;
                    visited[idx] = true;
                    for(auto x : v[i]) {
                        if(!visited[x]) unvisitedCount -= 1;
                        st.push(x);            
                    }
                }
            }
        } 
    }
    return pbc(unvisitedCount <= 0,__builtin_popcount(decisionInt));
}

int main(int args , char** kwargs) {
    int n , v, e;
    int max_threads = omp_get_max_threads();
    cin>>n>>v>>e;

    // input handler
    vector<vector<int>> adj(n);
    for(int i = 0 ; i < e ; i++) {
        int f, t;
        cin>>f>>t;
        adj[f].push_back(t);
        adj[t].push_back(f);
    }

    for(int i = 1 ; i < 100 ; i++) {
        pbc ret = checkCoverage(adj,i);
        cout<<ret.first<<" "<<ret.second;
    }

    return 0;
}