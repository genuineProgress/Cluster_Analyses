//
// Created by 1 on 22.04.2021.
//

#ifndef CLUSTERANALYSES_HIERARCHICAL_ALGORITHM_H
#define CLUSTERANALYSES_HIERARCHICAL_ALGORITHM_H
#include <vector>
#include "Field.h"
//#include "TreeNode.h"
//#include "Point.h"
class Cluster_Search;
class Field;
class Hierarchical_algorithm {
public:
    //void ha_merge_nodes (int a, int b, vector<TreeNode<const Point *> *> &tree_nodes);
    int process(vector<Cluster_Search *> cluster_searches_, Field *field);
    //Point *ha_get_new_node_center (TreeNode<const Point *> *&first, TreeNode<const Point *> *&second);
    //void ha_get_closest_nodes (int &a, int &b, const vector<TreeNode<const Point *> *> &tree_node);
};
#endif //CLUSTERANALYSES_HIERARCHICAL_ALGORITHM_H
