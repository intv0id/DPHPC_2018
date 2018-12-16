#include <stdio.h>
#include <algorithm>
#include <map>
#include <cstdlib>
#include <iostream>

#include "algorithms/boost_kruskal.hpp"
#include "algorithms/filter_kruskal.hpp"
#include <boost/graph/kruskal_min_spanning_tree.hpp>

#include <sys/time.h>

using namespace std;

l_edge_t boost_kruskal::algorithm(Graph &g, unsigned int n_threads) {

    boost::property_map<Boost_Graph,boost::edge_weight_t>::type w = get(boost::edge_weight, g.boost_rep);
    vector<Boost_Edge> v;

    struct timeval t0, t1;
    gettimeofday(&t0, NULL);

	kruskal_minimum_spanning_tree(g.boost_rep, back_inserter(v));
    gettimeofday(&t1, NULL);

    // std::cout << "BOOST: " << getTime(t1, t0) << std::endl;

	l_edge_t result;

	for (vector<Boost_Edge>::iterator it = v.begin(); it != v.end(); it++) {
		edge* e = new edge();
		e->source = boost::source(*it,g.boost_rep);
		e->target = boost::target(*it,g.boost_rep);
		e->weight=w[*it];
		result.push_back(e);
    }

    return result;
}
