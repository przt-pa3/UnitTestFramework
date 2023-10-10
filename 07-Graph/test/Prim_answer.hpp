#ifndef _PRIM_H
#define _PRIM_H

#include <optional>
#include <vector>
#include "Graph_answer.hpp"
#include <cfloat>
/* Feel free to add more standard library headers */

/* Returns the vector of edges in the MST, or std::nullopt if MST does
 * not exist */

inline std::optional<edges_t>
prim_minimum_spanning_tree(Graph& g, vertex_t src) {
	edges_t mst_vec;
	size_t v_num = g.ver_number();
	prdijk* prdijk_arr = new prdijk[v_num];

	for (size_t i = 0; i < v_num; ++i) {
		prdijk_arr[i] = std::make_tuple(false, false, 0, 0);
	}
	prdijk_arr[src] = std::make_tuple(true, true, 0, 0);
	vertex_t ver_vis = src;
	vertex_t ver_des = 0;
	std::list<ver_edg_t>::iterator iter;
	edge_weight_t min = DBL_MAX;
	vertex_t temp;
	edge_t mst_edge;
	bool imp;
	size_t cnt = 0;

	while (mst_vec.size() != v_num - 1) {
		for (iter = (g.vlist[ver_vis]).begin(); iter != (g.vlist[ver_vis]).end(); ++iter) {
			//get<0>(*iter);//destination
			//get<1>(*iter);//weight
			//prdijk_arr[get<0>(*iter)];
			if (!std::get<0>(prdijk_arr[std::get<0>(*iter)])) {
				if (!std::get<1>(prdijk_arr[std::get<0>(*iter)]) ||
					(std::get<1>(prdijk_arr[std::get<0>(*iter)]) && std::get<2>(prdijk_arr[std::get<0>(*iter)]) > std::get<1>(*iter))) {
					prdijk_arr[std::get<0>(*iter)] = std::make_tuple(false, true, std::get<1>(*iter), ver_vis);
				}
			}
		}
		min = DBL_MAX;
		imp = true;
		for (size_t i = 0; i < v_num; ++i) {
			if (std::get<1>(prdijk_arr[i]) && min > std::get<2>(prdijk_arr[i]) && i != ver_vis) {
				min = std::get<2>(prdijk_arr[i]);
				ver_des = i;
				temp = std::get<3>(prdijk_arr[i]);
				imp = false;
			}
		}

		if (min == DBL_MAX && imp && mst_vec.size() + 1 < v_num) {
			//cout << "Can't Visit Every Vertex -> No MST" << endl;
			return std::nullopt;
		}
		prdijk_arr[ver_des] = std::make_tuple(true, true, min, temp);
		mst_edge = std::make_tuple(temp, ver_des, min);
		mst_vec.push_back(mst_edge);
		prdijk_arr[ver_vis] = std::make_tuple(true, false, 0, 0);
		cnt++;
		ver_vis = ver_des;
	}
	delete[] prdijk_arr;
	return mst_vec;
}


#endif // _PRIM_H
