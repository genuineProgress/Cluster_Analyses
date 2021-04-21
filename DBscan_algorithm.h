//
// Created by 1 on 21.04.2021.
//

#ifndef CLUSTERANALYSES_DBSCAN_ALGORITHM_H
#define CLUSTERANALYSES_DBSCAN_ALGORITHM_H
#include <vector>
#include "Field.h"
class Cluster_Search;
class Field;
class DBscan_algorithm {
public:
    int process(vector <Cluster_Search *> cluster_searches_, Field *field, int search_id,int k);

};


#endif //CLUSTERANALYSES_DBSCAN_ALGORITHM_H
