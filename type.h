#pragma once
#include <set>

using namespace std;

namespace MDS
{
    typedef struct pbi 
    {
        set<int> S, X, I;
    } problemInstance;

}