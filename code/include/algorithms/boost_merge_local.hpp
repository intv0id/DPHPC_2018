#include "graph.hpp"
#include "common.hpp"
#include <vector>

#include <boost/graph/use_mpi.hpp>
#include <boost/graph/distributed/dehne_gotz_min_spanning_tree.hpp>

typedef boost::graph_traits<Boost_DistribGraph>::edge_descriptor DistriBoost_Edge;
typedef boost::graph_traits<Boost_DistribGraph>::vertex_descriptor DistriBoost_Vertex;

using namespace std;

class boost_merge_local : public mst_algorithm {
    public:
    boost_merge_local() : mst_algorithm("Boost_MergeLocal") {}
    virtual l_edge_t algorithm(Graph &g, unsigned int n_threads = 1);
};
