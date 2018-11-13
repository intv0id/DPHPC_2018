#include "graph.hpp"
#include "common.hpp"
#include <vector>

using namespace std;

int find_pivot(list<edge*> &edges);
list<edge*> filter(list<edge*> &edges, union_find *u_find);
pair<list<edge*>, list<edge*>> partition(list<edge*> &edges, int pivot);
l_edge_t seq_filter_kruskal_main(Graph &g, list<edge*> &edges, union_find *u);

class seq_filter_kruskal : public mst_algorithm{
	public:
	seq_filter_kruskal() : mst_algorithm("Sequential Filter Kruskal") {}
	virtual l_edge_t algorithm(Graph &g);
};
