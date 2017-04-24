//
// Created by Mark on 08.03.17.
//

#ifndef PURPLE_HDIST_H
#define PURPLE_HDIST_H

#include <string.h>
#include <algorithm>
#include <vector>
#include <limits.h>
#include "purple/JobInfo.h"

using namespace std;

namespace Purple {

    template<class _info_iterator, class _info = typename iterator_traits<_info_iterator>::value_type>
    void h_distribution(_info_iterator begin, _info_iterator end, size_t node_cnt) {
        sort(begin, end);

        int total_weight[node_cnt];
        memset(&total_weight, 0, sizeof(int) * node_cnt);

        for_each(begin, end, [&](_info &t) {
            long min_loaded = min_element(total_weight, total_weight + node_cnt) - total_weight;
            t.set_node_number((int) min_loaded);
            total_weight[min_loaded] += t.get_weight();
        });
    }

}


#endif //PURPLE_HDIST_H
