#include <stdio.h>
#include <algorithm>
#include <map>
#include <cstdlib>
#include <iostream>

#include "algorithms/boost_dense_boruvka.hpp"
#include <boost/graph/distributed/dehne_gotz_min_spanning_tree.hpp>


#include <sys/time.h>

using namespace std;

l_edge_t boost_dense_boruvka::algorithm(Graph &g, unsigned int n_threads) {

    boost::property_map<Boost_Graph,boost::edge_weight_t>::type w = get(boost::edge_weight, g.boost_rep);
    
    //boost::graph::parallel::process_group_type<Boost_DistribGraph>::type pg = boost::graph::parallel::process_group(g);


    vector<int> predMap(g.n);


    typedef boost::graph::parallel::process_group_type<DistribGraph>::type process_group_type;

    process_group_type pg = boost::graph::parallel::process_group(g.boost_rep);

    process_group_type::process_id_type id = boost::graph::parallel::process_id(pg);
    process_group_type::process_size_type p = boost::graph::parallel::num_processes(pg);    

    typedef boost::graph::parallel::property_map<Graph, vertex_index_t>::const_type seqIndexMap;
    typedef boost::graph::parallel::iterator_property_map<vector<int>::iterator, seqIndexMap> seqCentralityMap;

    vector<int> nonDistributedCentralityS(num_vertices(g.boost_rep), 0);
    seqCentralityMap nonDistributedCentrality(nonDistributedCentralityS.begin(), get(vertex_index, g.boost_rep));

    vector<int> nonDistributedWeightedCentralityS(num_vertices(g.boost_rep), 0);
    seqCentralityMap nonDistributedWeightedCentrality(nonDistributedWeightedCentralityS.begin(), get(vertex_index, g.boost_rep));



   
    //boost::parallel::distributed_property_map<boost::graph::distributed::mpi_process_group, boost::property_map<Boost_Graph,boost::edge_weight_t>, boost::edge_weight_t> dist_map = boost::parallel::make_distributed_property_map(boost::graph::distributed::mpi_process_group, w);
   
    vector<Boost_Edge> v;

   
   
   
   
    struct timeval t0, t1;
    gettimeofday(&t0, NULL);
    
	//dense_boruvka_minimum_spanning_tree(g.boost_rep, dist_map, back_inserter(v));
    gettimeofday(&t1, NULL);

	
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
