//
// Created by 1 on 22.04.2021.
//

#ifndef CLUSTERANALYSES_EM_ALGORITHM_H
#define CLUSTERANALYSES_EM_ALGORITHM_H
#include <vector>
#include "Field.h"

class Cluster_Search;
class Field;
class EM_algorithm {
public:
    int process(vector <Cluster_Search *> cluster_searches_, Field *field, int clusters_number);
    double N (const Point *a, vector<double> m, vector<double> Sgm);
};


#endif //CLUSTERANALYSES_EM_ALGORITHM_H
