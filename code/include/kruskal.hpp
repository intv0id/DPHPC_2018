#include "graph.hpp"
#include "common.hpp"
#include <vector>

using namespace std;

bool compare(const edge* a, const edge* b);
l_edge_t seq_kruskal_main(Graph &g, list<edge*> &edges, union_find* u_find);

class seq_kruskal : public mst_algorithm{
	public:
	seq_kruskal() : mst_algorithm("Sequential Kruskal") {}
	virtual l_edge_t algorithm(Graph &g);
};
