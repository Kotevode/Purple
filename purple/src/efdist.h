//
// Created by Mark on 08.03.17.
//

#ifndef PURPLE_EFDIST_H
#define PURPLE_EFDIST_H

#include "hdist.h"
#include <set>

using namespace std;

namespace Purple {

    template<class _info_iterator>
    bool strong_rule(_info_iterator begin, _info_iterator end, int min_node_index, int max_node_index, int &delta) {
        if (delta == 0) return false;
        for (_info_iterator min = begin; min < end; min++) {
            if (min->get_node_number() == max_node_index &&
                min->get_weight() < delta) {
                min->set_node_number(min_node_index);
                delta = min->get_weight();
                return true;
            }
        }
        return false;
    }

    template<class _info_iterator>
    bool weak_rule(_info_iterator begin, _info_iterator end, int min_node_index, int max_node_index, int &delta) {
        if (delta == 0) return false;
        for (_info_iterator min = begin; min < end; min++)
            for (_info_iterator max = begin; max < end; max++) {
                if (min->get_node_number() == min_node_index &&
                    max->get_node_number() == max_node_index &&
                    max->get_weight() - min->get_weight() < delta &&
                    max->get_weight() - min->get_weight() > 0) {
                    min->set_node_number(max_node_index);
                    max->set_node_number(min_node_index);
                    delta = max->get_weight() - min->get_weight();
                    return true;
                }

            }
        return false;
    }


    template<class _info_iterator, class _info = typename iterator_traits<_info_iterator>::value_type>
    void ef_distribution(_info_iterator begin, _info_iterator end, size_t node_cnt) {
        h_distribution(begin, end, node_cnt);

        int node_load[node_cnt];
        memset(node_load, 0, sizeof(int) * node_cnt);
        for_each(begin, end, [&](_info &t) {
            node_load[t.get_node_number()] += t.get_weight();
        });

        set<int> not_min;
        for (int i = 0; i < node_cnt; i++) not_min.insert(i);
        while (true) {
            if (not_min.size() == 1) return;
            auto min_and_max = minmax_element(not_min.begin(), not_min.end(), [&](int a, int b) -> bool {
                return node_load[a] < node_load[b];
            });

            int min_loaded_index = *(min_and_max.first);
            int max_loaded_index = *(min_and_max.second);

            int delta = node_load[max_loaded_index] - node_load[min_loaded_index];

            if (strong_rule(begin, end, min_loaded_index, max_loaded_index, delta) ||
                weak_rule(begin, end, min_loaded_index, max_loaded_index, delta)) {
                for (int i = 0; i < node_cnt; i++) not_min.insert(i);
                node_load[min_loaded_index] += delta;
                node_load[max_loaded_index] -= delta;
            } else {
                not_min.erase(min_loaded_index);
            }
        }
    }
}

#endif //PURPLE_EFDIST_H
