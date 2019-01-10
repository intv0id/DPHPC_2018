#include "graph.hpp"
#include "common.hpp"
#include <vector>

#include <boost/graph/use_mpi.hpp>
#include <boost/graph/distributed/dehne_gotz_min_spanning_tree.hpp>

typedef boost::graph_traits<Boost_Graph>::edge_descriptor Boost_Edge;
using namespace std;

class boost_dense_boruvka : public mst_algorithm {
	public:
	boost_dense_boruvka() : mst_algorithm("Boost_DenseBoruvka") {}
	virtual l_edge_t algorithm(Graph &g, unsigned int n_threads = 1);
};
