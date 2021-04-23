//
// Created by 1 on 23.04.2021.
//

#ifndef CLUSTERANALYSES_FOREL_ALGORITHM_H
#define CLUSTERANALYSES_FOREL_ALGORITHM_H
#include <vector>
#include "Field.h"
class Cluster_Search;
class Field;
class Forel_algorithm {
public:
    int process(vector<Cluster_Search *> cluster_searches_, Field *field_);

};


#endif //CLUSTERANALYSES_FOREL_ALGORITHM_H
