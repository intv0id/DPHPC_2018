#include <stdio.h>
#include <algorithm>
#include <map>
#include <cstdlib>
#include <iostream>

#include "algorithms/boost_dense_boruvka.hpp"
#include <boost/graph/distributed/dehne_gotz_min_spanning_tree.hpp>
#include <boost/graph/distributed/vertex_list_adaptor.hpp>

#include <sys/time.h>

using namespace std;

l_edge_t boost_dense_boruvka::algorithm(Graph &g, unsigned int n_threads) {

    boost::property_map<Boost_DistribGraph, boost::edge_weight_t>::type w = get(boost::edge_weight, g.boost_distrib_rep);
    
    vector<Boost_Edge> v;
   
    struct timeval t0, t1;
    gettimeofday(&t0, NULL);
    
	dense_boruvka_minimum_spanning_tree(make_vertex_list_adaptor(g.boost_distrib_rep), w, back_inserter(v));
    gettimeofday(&t1, NULL);

	
    l_edge_t result;

	for (vector<Boost_Edge>::iterator it = v.begin(); it != v.end(); it++) {
		
       if (process_id(g.boost_distrib_rep.process_group()) == 0) {
          edge* e = new edge();
	      Boost_Vertex u = boost::source(*it,g.boost_distrib_rep);
		  Boost_Vertex v = boost::target(*it,g.boost_distrib_rep);
		  e->weight=w[*it];
		  result.push_back(e);
       }
   }

    return result;
}
