#include "graph.hpp"
#include "common.hpp"

#include <vector>

using namespace std;

class parallel_sollin_AL : public mst_algorithm{
	public:
	virtual l_edge_t algorithm(Graph& g);
	parallel_sollin_AL(string name) :mst_algorithm(name){}
};

class parallel_sollin_EL : public mst_algorithm{
	public :
	virtual l_edge_t algorithm(Graph& g);
	parallel_sollin_EL(string name) :mst_algorithm(name){}
};
