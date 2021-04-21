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
       // cluster_searches_[search_id]->wave();
        vector<bool> burnt (Point::quantity (), false);
      // that cycle checks if point in marked cluster and if not creates new one
        for (int m = 0; m < Point::quantity (); ++m) {
          if (burnt[m]) {
              continue;
          }
          vector<int> curr_wave = {m};
          burnt[m] = true;
          vector<int> next_wave; // points, connected to smth from curr_wave
          vector<int> curr_cluster; // points from current cluster
          // that cycle searches for neighbours of points in curr_wave
          while (!curr_wave.empty ()) {
              for (int i : curr_wave) {
                  for (int j = m + 1; j < Point::quantity (); ++j) {
                      if (cluster_searches_[search_id]->edges ()[i][j] && !burnt[j]) {
                          next_wave.push_back (j);
                          burnt[j]=true;
                      }
                  }
              }
              curr_cluster.insert (curr_cluster.end (), curr_wave.begin (), curr_wave.end ());
              curr_wave = next_wave;
              for (auto &&i: next_wave) {
                  burnt[i] = true;
              }
              next_wave.clear ();
          }
          cluster_searches_[search_id]->add (Cluster_Search::Cluster (curr_cluster));
      }
        return 0;

    }
