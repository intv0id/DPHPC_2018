#include <stdio.h>
#include <algorithm>
#include <map>
#include <cstdlib>
#include <iostream>

#include "algorithms/boost_prim.hpp"
#include "algorithms/filter_kruskal.hpp"
#include <boost/graph/prim_minimum_spanning_tree.hpp>

#include <sys/time.h>

using namespace std;

l_edge_t boost_prim::algorithm(Graph &g, unsigned int n_threads) {

    bool debug = false;

    boost::property_map<Boost_Graph,boost::edge_weight_t>::type w = get(boost::edge_weight, g.boost_rep);
    vector<Boost_Edge> v;

    struct timeval t0, t1;
    vector<int> predMap(g.n);

    gettimeofday(&t0, NULL);
    prim_minimum_spanning_tree(g.boost_rep, boost::make_iterator_property_map(predMap.begin(), get(boost::vertex_index, g.boost_rep)));
	// prim_minimum_spanning_tree(g.boost_rep, back_inserter(v));
    gettimeofday(&t1, NULL);

    if (debug) {
        std::cout << "BOOST PRIM: " << getTime(t1, t0) << std::endl;
    }

	l_edge_t result;
    bool success;
    boost::graph_traits<Boost_Graph>::edge_descriptor e_boost;

    if (debug) {
        cout << "n = " << g.n << endl;
        cout << predMap[0] << " -> 0" << endl;
        cout << predMap[11] << " -> 11" << endl;
        cout << predMap[1362] << " -> 1362" << endl;
    }

    for (int vertex = 0; vertex != g.n; vertex++){

        if (debug) {

            if (predMap[vertex] == 0) {
                cout << "0 goes to: " << vertex << endl;
            }
            if (predMap[vertex] == 11) {
                // cout << "11 goes to: " << vertex << endl;
            }
            if (predMap[vertex] == 1362) {
                // cout << "1362 goes to: " << vertex << endl;
            }
        }
        
        if (vertex != predMap[vertex]) {

            boost::tie(e_boost, success) = boost::edge(predMap[vertex],vertex, g.boost_rep);
            assert(success);

            edge* e = new edge();
            e->source = predMap[vertex];
            e->target = vertex;
            e->weight = 9999999;

            for (edge* ed: g.adjacency_list[e->source]->adjacent_vertices) {
                if (ed->target == vertex) {
                    e->weight = min(ed->weight, e->weight);
                }
            }

            result.push_back(e);
        }
        else {
            if (debug) {
                cout << "KOIS: " << vertex << " -> " << predMap[vertex] << endl;
            }
        }
    }   

    return result;
}
