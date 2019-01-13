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
    
    vector<DistriBoost_Edge> v;
   
    struct timeval t0, t1;
    gettimeofday(&t0, NULL);
    
    dense_boruvka_minimum_spanning_tree(make_vertex_list_adaptor(g.boost_distrib_rep), w, back_inserter(v));
    gettimeofday(&t1, NULL);

    
    l_edge_t result;
    cout << "dense boruv end, v size: " << v.size() << endl;
    for (vector<DistriBoost_Edge>::iterator it = v.begin(); it != v.end(); it++) {
       cout << "iter edge" << endl; 
       if (process_id(g.boost_distrib_rep.process_group()) == 0) {
          edge* e = new edge();
          DistriBoost_Vertex u = boost::source(*it,g.boost_distrib_rep);
          DistriBoost_Vertex v = boost::target(*it,g.boost_distrib_rep);
	  //VertexIndexMap indexMap = get(vertex_index, g.boost_distrib_rep);
      	  cout << "test" << endl;
	  cout << "(" << g.boost_distrib_rep.distribution().global(owner(u), local(u))
                << ", " << g.boost_distrib_rep.distribution().global(owner(v), local(v))<< ")\n";
	  e->weight=w[*it];
          result.push_back(e);
       }
   }

    return result;
}
