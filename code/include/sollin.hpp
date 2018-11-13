#include "graph.hpp"
#include "common.hpp"

#include <vector>

using namespace std;

class sollin : public mst_algorithm {
	public:
	virtual l_edge_t algorithm(Graph& g);
	sollin(string name) : mst_algorithm(name){}
};
