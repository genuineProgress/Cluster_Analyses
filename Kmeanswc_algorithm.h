//
// Created by 1 on 22.04.2021.
//

#ifndef CLUSTERANALYSES_KMEANSWC_ALGORITHM_H
#define CLUSTERANALYSES_KMEANSWC_ALGORITHM_H
#include <vector>
#include "Field.h"
class Cluster_Search;
class Field;
class Kmeanswc_algorithm {
public:
    int process(vector<Cluster_Search*> cluster_searches_,Field *field_,int clusters_number,int cores_number);

};


#endif //CLUSTERANALYSES_KMEANSWC_ALGORITHM_H
