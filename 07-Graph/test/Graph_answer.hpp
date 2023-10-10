#ifndef _GRAPH_H
#define _GRAPH_H

#include <vector>
#include <unordered_map>
#include <list>
#include <tuple>

/* Feel free to add more standard library headers */

// A vertex is typed as `vertex_t`. An edge is typed as `edge_t`,
// which is associated with a source vertex, a destination vertex, and
// its weight. A list of edges is typed as `edges_t`, which is a
// vector of `edge_t`.

using vertex_t = std::size_t;
using edge_weight_t = double;
using edge_t = std::tuple<vertex_t,
	vertex_t,
	edge_weight_t>;
using edges_t = std::vector<edge_t>;
using prdijk = std::tuple<bool,
	bool,
	edge_weight_t,
	vertex_t>;

using ver_edg_t = std::tuple < vertex_t,
	edge_weight_t>;

enum class GraphType {
	UNDIRECTED,
	DIRECTED
};

/*
 * We do not enforce a specific design constraints other than the
 * consturtor. You may introduce your own changes as you
 * want---implementing your own member functions or variables. You
 * will still need to ensure that your constructor works properly,
 * such that we can run the test for grading. The provided test will
 * check if the constructor is correctly designed.
 */
class Graph {
public:
	Graph() = delete;
	Graph(const Graph&) = delete;
	Graph(Graph&&) = delete;
	size_t num_ver;
	std::list<ver_edg_t>* vlist;

	/* We assume that if num_vertices is n, a graph contains n vertices
	   from 0 to n-1. */
	Graph(size_t num_vertices,
		const edges_t& edges,
		GraphType Type) {
		// TODO: Implement the constructor
		num_ver = num_vertices;
		vlist = new std::list<ver_edg_t>[num_ver];
		ver_edg_t temp;
		size_t edge_size = edges.size();
		vertex_t v_s;
		vertex_t v_d;
		edge_weight_t e_w;
		for (size_t i = 0; i < edge_size; i++) {
			v_s = std::get<0>(edges[i]);  // source vertex
			v_d = std::get<1>(edges[i]);  // destination vertex
			e_w = std::get<2>(edges[i]);  // weight
			temp = std::make_tuple(v_d, e_w);
			if (Type == GraphType::DIRECTED) {
				vlist[v_s].push_back(temp);
			}
			else {
				vlist[v_s].push_back(temp);
				ver_edg_t temp = std::make_tuple(v_s, e_w);
				vlist[v_d].push_back(temp);
			}
		}
	}

	~Graph() {
		delete[] vlist;
	}

	// TODO(optional): Define helper functions, e.g., out_deges(v)
	inline size_t in_degres(vertex_t v);
	inline size_t out_degres(vertex_t v);
	inline size_t ver_number();
private:
	// TODO: Roll out your own data structures
};

size_t Graph::ver_number() {
	return num_ver;
}

size_t Graph::in_degres(vertex_t v) {
	size_t cnt = 0;
	for (size_t i = 0; i < num_ver; ++i) {
		if (i != v) {
			std::list<ver_edg_t>::iterator iter;
			for (iter = vlist[i].begin(); iter != vlist[i].end(); ++iter) {
				if (std::get<0>(*iter) == v)
					cnt++;
			}
		}
		else
			continue;
	}
	return cnt;
}

size_t Graph::out_degres(vertex_t v) {
	return vlist[v].size();;
}


#endif // _GRAPH_H
