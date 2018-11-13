#include <iostream>
#include <fstream>
#include <string>

#include <omp.h>

#include "graph.hpp"
#include "timer.hpp"

#include <boost/graph/kruskal_min_spanning_tree.hpp>
#include <boost/graph/prim_minimum_spanning_tree.hpp>

typedef boost::graph_traits<Boost_Graph>::edge_descriptor Boost_Edge;
using namespace std;

Timer::Timer(string name,list<mst_algorithm*> l) : algorithms(l)  
{
	o.open(name);
}

void Timer::printF(string field, string s) {
	o << field << ": " <<s << endl;
	cout << field << ": " << s << endl;
}

void Timer::printF(string field, int s) {
	o << field << ": " <<s << endl;
	cout << field << ": " << s << endl;
}

void Timer::time(Graph& g) {
	double time1,time2;
	for(mst_algorithm* mst_alg : algorithms){
		mst_algorithm& mst_algo = *mst_alg;
		
		double time1 = omp_get_wtime();
		l_edge_t mst = mst_algo.algorithm(g);
		double time2 = omp_get_wtime();
		
		o << mst_algo.name << " " << time2 - time1 << endl;

		int weight(0);
		for(l_edge_it it = mst.begin(); it != mst.end(); it++){
			weight += (*it)->weight;
		}
		cout << "Name: " << mst_algo.name << " Weight " << weight << endl;
	}
	// Apply boost algorithm
	boost::property_map<Boost_Graph,boost::edge_weight_t>::type w = get(boost::edge_weight,g.boost_rep);
	vector<Boost_Edge> v;

	// Kruskal
	time1 = omp_get_wtime();
	kruskal_minimum_spanning_tree(g.boost_rep,back_inserter(v));
	time2 = omp_get_wtime();
	o << time2 - time1 << " ";

	// Print all weights
	int weight_kruskal_boost = 0;
	for(vector<Boost_Edge>::iterator it = v.begin(); it != v.end(); it++){
		weight_kruskal_boost += w[*it];
	}
	cout << "Weight Kruskal Boost: " << weight_kruskal_boost << endl;

	// Prim
	vector<int> predMap(g.n);
	time1 = omp_get_wtime();
	prim_minimum_spanning_tree(g.boost_rep,boost::make_iterator_property_map(predMap.begin(),get(boost::vertex_index,g.boost_rep)));
	time2 = omp_get_wtime();
	o << time2 - time1 << " ";

	// Print all weights
	int weight_prim_boost = 0;
	for(int vertex = 1; vertex != g.n; vertex++){
		bool success;
		boost::graph_traits<Boost_Graph>::edge_descriptor e;
		boost::tie(e,success) = boost::edge(predMap[vertex],vertex,g.boost_rep);
		assert(success);
		weight_prim_boost += w[e];
	}
	cout << "Weight Prim Boost: " << weight_prim_boost << endl;

	o << endl;

};
