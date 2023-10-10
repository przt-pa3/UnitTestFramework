#ifndef _TOPOLOGICAL_H
#define _TOPOLOGICAL_H

#include <vector>
#include <list>
#include "Graph_answer.hpp"
/* Feel free to add more standard library headers */

/* Return _a_ valid topologically sorted sequence of vertex descriptors */
inline std::vector<vertex_t> topological_sort(Graph& g) {
	std::list<size_t> queued;
	size_t v_num = g.ver_number();
	std::vector<vertex_t> sort_result;
	size_t* in_deg_arr = new size_t[v_num];
	for (size_t i = 0; i < v_num; ++i) {
		in_deg_arr[i] = g.in_degres(i);
	}
	size_t popver;
	while (sort_result.size() != v_num) {
		if (queued.size() == 0) {
			for (size_t i = 0; i < v_num; ++i) {
				if (in_deg_arr[i] == 0) {
					queued.push_back(i);
				}
			}
		}
		else {
			popver = queued.front();
			queued.pop_front();
			std::list<ver_edg_t>::iterator iter;
			for (iter = (g.vlist[popver]).begin(); iter != (g.vlist[popver]).end(); ++iter) {
				in_deg_arr[std::get<0>(*iter)]--;
			}
			sort_result.push_back(popver);
			in_deg_arr[popver] = -1;
		}
	}
	delete[] in_deg_arr;
	return sort_result;
}


#endif // _TOPOLOGICAL_H
