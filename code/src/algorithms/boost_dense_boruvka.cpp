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

    l_edge_t result;

    boost::property_map<Boost_DistribGraph, boost::edge_weight_t>::type w = get(boost::edge_weight, g.boost_distrib_rep);
    
    vector<DistriBoost_Edge> v;
   
    dense_boruvka_minimum_spanning_tree(make_vertex_list_adaptor(g.boost_distrib_rep), w, back_inserter(v));

    for (vector<DistriBoost_Edge>::iterator it = v.begin(); it != v.end(); it++) {
       if (process_id(g.boost_distrib_rep.process_group()) == 0) {
          edge* e = new edge();
          DistriBoost_Vertex u1 = boost::source(*it, g.boost_distrib_rep);
          DistriBoost_Vertex u2 = boost::target(*it, g.boost_distrib_rep);
      
          e->source=g.boost_distrib_rep.distribution().global(owner(u1), local(u1));
          e->target=g.boost_distrib_rep.distribution().global(owner(u2), local(u2));
          e->weight=w[*it];
          result.push_back(e);
       }
   }
    //}

    return result;
}
