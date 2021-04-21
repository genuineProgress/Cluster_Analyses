#include <__bit_reference>
//
// Created by 1 on 16.04.2021.
//

#include "Wave_algorithm.h"
/*
 *
int Field::wave_clustering (int search_id) {
    // if not readonly state returns -1
    // if readonly - clustering
    /* Errors
     * -1 field is not in readonly mode
     * -3 There is no such Cluster_Search
     */
  /*  if (!readonly ()) {
        return -1;
    }
    if (search_id >= searches ().size ()) {
        return -3;
    }
    searches_[search_id].wave ();
    return 0;
}

*/


int Wave_algorithm::process(vector<Cluster_Search *> cluster_searches_, Field *field, int search_id) {
      // if not readonly state returns -1
      // if readonly - clustering
      /* Errors
       * -1 field is not in readonly mode
       * -3 There is no such Cluster_Search
       */
       if (!field->readonly()) {
            return -1;
        }
        if (search_id >= cluster_searches_.size ()) {
            return -3;
        }
        cluster_searches_[search_id]->wave();
        return 0;

    }
