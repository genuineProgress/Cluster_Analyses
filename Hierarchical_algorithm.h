//
// Created by 1 on 22.04.2021.
//

#ifndef CLUSTERANALYSES_HIERARCHICAL_ALGORITHM_H
#define CLUSTERANALYSES_HIERARCHICAL_ALGORITHM_H
#include <vector>
#include "Field.h"
#include "TreeNode.h"
#include "Cluster_Search.h"
//#include "Point.h"
class Cluster_Search;
class Field;
class Hierarchical_algorithm {
private: Field *field_;
public:
    explicit Hierarchical_algorithm (Field *field);
    void ha_merge_nodes (int a, int b, vector<TreeNode<const Point *> *> &tree_nodes);
    int process(vector<Cluster_Search *> cluster_searches_, Field *field);
    Point *ha_get_new_node_center (TreeNode<const Point *> *&first, TreeNode<const Point *> *&second);
    void ha_get_closest_nodes (int &a, int &b, const vector<TreeNode<const Point *> *> &tree_node);
    void ha_get_node_sum (TreeNode<const Point *> *&node, double &sum_x, double &sum_y, int &points);
    void ha_fprintf (const vector<TreeNode<const Point *> *> &tree_nodes,
                                             int iteration,
                                             const Point &old_1,
                                             const Point &old_2,
                                             ofstream &tree);

    };
#endif //CLUSTERANALYSES_HIERARCHICAL_ALGORITHM_H
