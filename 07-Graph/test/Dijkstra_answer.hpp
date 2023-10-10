#ifndef _DIJKSTRA_H
#define _DIJKSTRA_H

#include <unordered_map>
#include <optional>
#include <tuple>
#include <cfloat>
/* Feel free to add more standard library headers */

#include "Graph_answer.hpp"

/* Given a vertex `v`, `dijkstra_shortest_path` is:
 * - `nullopt` if `v` is not reacheble from `src`.
 * - {`u`, `d`} where `u` is the predecessor of `v` and `d` is the distance
 *   from `src`
 */

inline std::unordered_map<vertex_t,
    std::optional<std::tuple<vertex_t, edge_weight_t>>>
    dijkstra_shortest_path(Graph& g, vertex_t src) {

    std::unordered_map<vertex_t,
        std::optional<std::tuple<vertex_t, edge_weight_t>>> ans;

    vertex_t v_des;
    vertex_t v_vis;


    size_t v_num = g.ver_number();
    bool* v_arr_fin = new bool[v_num];
    prdijk* prdijk_arr = new prdijk[v_num];
    std::list<ver_edg_t>::iterator iter;
    edge_weight_t min = DBL_MAX;
    bool imp;
    vertex_t temp;

    size_t cnt = 0;
    for (size_t i = 0; i < v_num; ++i) {
        prdijk_arr[i] = std::make_tuple(false, false, 0, 0);
        v_arr_fin[i] = false;
    }
    prdijk_arr[src] = std::make_tuple(true, true, 0, 0);
    v_vis = src;
    v_des = src;

    //start iteration
    while (cnt != v_num - 1) {
        for (iter = (g.vlist[v_vis]).begin(); iter != (g.vlist[v_vis]).end(); ++iter) {

            if (!std::get<0>(prdijk_arr[std::get<0>(*iter)]) && !v_arr_fin[std::get<0>(*iter)]) {
                if (!std::get<1>(prdijk_arr[std::get<0>(*iter)]) ||
                    (std::get<1>(prdijk_arr[std::get<0>(*iter)]) &&
                        std::get<2>(prdijk_arr[std::get<0>(*iter)]) >
                        std::get<1>(*iter) + std::get<2>(prdijk_arr[v_vis]))) {
                    prdijk_arr[std::get<0>(*iter)] = std::make_tuple(false, true,
                        std::get<1>(*iter) + std::get<2>(prdijk_arr[v_vis]), v_vis);
                }
            }
        }

        min = DBL_MAX;
        imp = true;
        for (size_t i = 0; i < v_num; ++i) {
            if (std::get<1>(prdijk_arr[i]) && min > std::get<2>(prdijk_arr[i]) && i != v_vis && !v_arr_fin[i]) {
                min = std::get<2>(prdijk_arr[i]);
                v_des = i;
                temp = std::get<3>(prdijk_arr[i]);
                imp = false;
            }
        }
        if (min == DBL_MAX && imp && ans.size() + 1 < v_num) { break; }
        prdijk_arr[v_des] = std::make_tuple(true, true, min, temp);
        v_arr_fin[v_vis] = true;
        ans[v_vis] = std::make_tuple(std::get<3>(prdijk_arr[v_vis]), std::get<2>(prdijk_arr[v_vis]));
        cnt++;
        if (v_arr_fin[v_des]) break;
        v_vis = v_des;
    }
    ans[v_vis] = std::make_tuple(std::get<3>(prdijk_arr[v_vis]), std::get<2>(prdijk_arr[v_vis]));
    v_arr_fin[v_vis] = true;

    for (size_t i = 0; i < v_num; ++i) {
        if (!std::get<0>(prdijk_arr[i]) && !std::get<1>(prdijk_arr[i]))
            ans[i] = std::nullopt;
    }
    delete[] v_arr_fin;
    delete[] prdijk_arr;
    return ans;
}


#endif // _DIJKSTRA_H
