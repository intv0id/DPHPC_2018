#include "common.hpp"
#include "graph.hpp" 

#include <iostream>
#include <vector>

using namespace std;

void print_edge_EL_list(l_edge_EL_t mst){
	cout << endl;
	for(l_edge_EL_it ite = mst.begin(); ite != mst.end(); ite++){
		cout << "(" << (*ite)->source << "," << (*ite)-> target << ")" << endl;
	}
	cout << endl;
}
