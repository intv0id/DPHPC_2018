#include "graph.hpp"
#include "common.hpp"
#include <vector>

using namespace std;

bool compare(const edge* a, const edge* b);
l_edge_t kruskal_main(Graph &g, vector<edge*> &edges, union_find* u_find);
l_edge_t seq_kruskal(Graph &g);
l_edge_t par_kruskal(Graph &g);
