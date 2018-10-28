#include "common.hpp"
#include "graph.hpp" 

#include <iostream>
#include <vector>

using namespace std;

union_find::union_find(int n) : parents(n), size(n), numTrees(n) {
    for(int i = 0; i != n; i++){
        parents[i] = i;
    }
}

int union_find::find(int x){
    if(parents[x] != x){
        parents[x] = find(parents[x]);
        return parents[x];
    }
    return x;
}

bool union_find::unite(int x, int y){
    int px = find(x);
    int py = find(y);
    if(px != py){
        parents[py] = px;
        numTrees--;
        return true;
    }
    return false;
}

void print_edge_list(l_edge_t mst){
    int c = 0;
	cout << endl;
	for(l_edge_it ite = mst.begin(); ite != mst.end(); ite++){
        c += (*ite)->weight;
		cout << "(" << (*ite)->source << "," << (*ite)-> target << ")" << endl;
	}
	cout << endl;
    cout << "Cout total : " << c << endl;
}
