//
// Created by 1 on 21.04.2021.
//

#include "DBscan_algorithm.h"

int DBscan_algorithm::process(vector<Cluster_Search *> cluster_searches_, Field *field, int search_id, int k) {
    /* Errors
 * -1 field is not in readonly mode
 * -3 There is no such Cluster_Search
 */
    if (!field->readonly ()) {
        return -1;
    }
    if (search_id >= cluster_searches_.size ()) {
        return -3;
    }
    cluster_searches_[search_id]->dbscan (k);
    return 0;

}
