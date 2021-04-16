//
// Created by 1 on 16.04.2021.
//

#ifndef CLUSTERANALYSES_WAVE_ALGORITHM_H
#define CLUSTERANALYSES_WAVE_ALGORITHM_H
#include <vector>
#include "Field.h"
using namespace std;
class Cluster_Search;
class Field;
class Wave_algorithm {
public:
    int process(vector <Cluster_Search *> cluster_searches_, Field *field, int search_id);

};


#endif //CLUSTERANALYSES_WAVE_ALGORITHM_H
