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
    //db_clustering (db_sorting (density));
    // counts number of neighbours for all the points

    vector<int> type (Point::quantity (), 0);
    for (int i = 0; i < Point::quantity (); ++i) {
        vector<int> neighbours;
        for (int j = 0; j < Point::quantity (); ++j) {
            if (cluster_searches_[search_id]->edges ()[i][j]) {
                neighbours.push_back (j);
            }
        }
        if (neighbours.size () >= k) {
            type[i] = 2;
            for (int neighbour : neighbours) {
                if (type[neighbour] != 2) {
                    type[neighbour] = 1;
                }
            }
        }
    }
    vector<int> state=type;
    // picks up core points and finds its cluster
    // saves info about the states of points to file
    // result of clustering saved to file too
    vector<bool> burnt (Point::quantity (), false);
    for (int l = 0; l < state.size (); ++l) {
        if (burnt[l]) {
            continue;
        }
        vector<int> curr_cluster;
        if (state[l] == 2) {
            vector<int> curr_added;
            curr_added.push_back (l);
            vector<int> new_added;
            while (!curr_added.empty ()) {
                for (int i : curr_added) {
                    for (int j = 0; j < Point::quantity (); ++j) {
                        if (!burnt[j] && i != j && cluster_searches_[search_id]->edges ()[i][j]) {
                            new_added.push_back (j);
                            burnt[j] = true;
                        }
                    }
                }
                curr_cluster.insert (curr_cluster.end (), new_added.begin (), new_added.end ());
                curr_added = new_added;
                new_added.clear ();
            }
            //cluster_searches_[search_id]->add (Cluster_Search::Cluster (curr_cluster));
           cluster_searches_[search_id]->clusters.emplace_back (curr_cluster); //this version is from working previous state
        } else if (state[l] == 0) {
            // Doesn't seem to be true.
            // That point can be connected to core point which has id number more than ours
            //cluster_searches_[search_id]->add (vector<int> (1,l));
            cluster_searches_[search_id]->clusters.emplace_back (vector<int> (1, l)); //this version is from working previous state
            burnt[l] = true;
        }
    }
    //dbscan_fprintf_type (state);
    //fprintf_clusters();
    cluster_searches_[search_id]->dbscan (k);
    return 0;

}
