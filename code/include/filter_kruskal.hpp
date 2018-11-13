#include "graph.hpp"
#include "common.hpp"
#include <vector>

using namespace std;

int find_pivot(vector<edge*> &edges);
vector<edge*> filter(vector<edge*> &edges, union_find *u_find);
pair<vector<edge*>, vector<edge*>> partition(vector<edge*> &edges, int pivot);
l_edge_t seq_filter_kruskal_main(Graph &g, vector<edge*> &edges, union_find *u);
l_edge_t seq_filter_kruskal(Graph &g);
