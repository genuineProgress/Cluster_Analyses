//
// Created by 1 on 22.04.2021.
//

#include "Hierarchical_algorithm.h"

int Hierarchical_algorithm::process(vector<Cluster_Search *> cluster_searches_, Field *field) {
    /* Errors
        * -1 field is not in readonly mode
        */
    if (!field->readonly ()) {
        return -1;
    }
    int tree_number = Point::quantity ();
    vector<TreeNode<const Point *> *> tree_nodes;
    tree_nodes.reserve (Point::quantity ());
    ofstream tree ("gnuplot/ha/ha_tree.txt");
    for (int p = 0; p < Point::quantity (); ++p) {
        tree_nodes.push_back (new TreeNode<const Point *> (Point::get_by_id (p + 1)));
    }
    while (1 < tree_nodes.size ()) {
        int a, b;
        //ha_get_closest_nodes (a, b, tree_nodes);
        Point old_1 (tree_nodes[a]->value ()->x (), tree_nodes[a]->value ()->y (), 0);
        Point old_2 (tree_nodes[b]->value ()->x (), tree_nodes[b]->value ()->y (), 0);
        //ha_merge_nodes (a, b, tree_nodes);
        //ha_fprintf (tree_nodes, Point::quantity () - tree_nodes.size (), old_1, old_2, tree);
    }
    //tree_root_ = tree_nodes[0];

}

