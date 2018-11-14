#include "graph.hpp"
#include "common.hpp"
#include <vector>

using namespace std;

class seq_prim : public mst_algorithm{
	public:
	seq_prim() : mst_algorithm("Sequential Prim") {}
	virtual l_edge_t algorithm(Graph &g);
};
