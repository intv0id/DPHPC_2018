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
    int parent = parents[x];
    if(parent != x){
        parent = find(parents[x]);
	if(parent != parents[x]){
		parents[x] = parent;
	}
        return parents[x];
    }
    return x;
}

int union_find::find_debug(int x){
    int parent = parents[x];
    if(parent != x){
        parent = find(parents[x]);
	if(parent != parents[x]){
		cout << "Writing: " << x << endl;
		parents[x] = parent;
	}
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

void union_find::update_parents(){
	for(unsigned int i = 0; i < size; i++){
		find(i);
	}
}

comp::comp(union_find* u_): u(u_){}
compVertex::compVertex(union_find* u_): u(u_){}
compVertexFAL::compVertexFAL(union_find* u_): u(u_){}
compTargetVertex::compTargetVertex(union_find* u_): u(u_){}

bool comp::operator()(edge*e1, edge* e2) const {
	int p1 = u->find(e1->source);
	int p2 = u->find(e2->source);
	if(p1 < p2) return true;
	if(p1 > p2) return false;
	int p3 = u->find(e1->target);
	int p4 = u->find(e2->target);
	if(p3 < p4) return true;
	if(p3 > p4) return false;
	int w1 = e1->weight;
	int w2 = e2->weight;
	return (w1 < w2);
}

bool compVertex::operator()(const vertex_adjacency_list* v1,const vertex_adjacency_list* v2) const {
	int p1 = u->find(v1->index);
	int p2 = u->find(v2->index);
	return p1 < p2;
}

bool compVertexFAL::operator()(const component_FAL* v1,const component_FAL* v2) const {
	int p1 = u->find(v1->index);
	int p2 = u->find(v2->index);
	return p1 < p2;
}
bool compTargetVertex::operator()(edge* v1, edge* v2) const {
	int p1 = u->find(v1->target);
	int p2 = u->find(v2->target);
	if (p1 < p2) return true;
	else if(p1>p2) return false;
	else return v1->weight < v2->weight;
}

bool compWeight::operator()(edge* v1, edge* v2) const {
	return v1->weight < v2->weight;
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

string mst_algorithm::get_measurements_names(){
	stringstream ss;
	for(auto x : internal_timings){
		ss << x.name << " ";
	}
	return ss.str();
}

string mst_algorithm::get_measurements_values(){
	stringstream ss;
	for(auto x : internal_timings){
		ss << x.measurement << " ";
	}
	return ss.str();
}
