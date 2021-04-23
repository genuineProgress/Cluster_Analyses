//
// Created by 1 on 23.04.2021.
//

#include "Forel_algorithm.h"

int Forel_algorithm::process(vector<Cluster_Search *> cluster_searches_, Field *field_) {
    /* Errors
    * -1 field is not in readonly mode
    */
    if (!field_->readonly ()) {
        return -1;
    }
    // realises the forel algorithm
    vector<TreeNode<Point> *> center_nodes;
    double R = 0.05;
    for (int p = 0; p < Point::quantity (); ++p) {
        auto *temp = new TreeNode<Point> (Point (Point::get_by_id (p + 1)->x (), Point::get_by_id (p + 1)->y (), 0));
        center_nodes.push_back (temp);
    }
    int print_number = 0;
    while (center_nodes.size () > 1) {
        vector<bool> clustered (center_nodes.size (), false);
        vector<TreeNode<Point> *> new_center_nodes;
        for (int p = 0; p < center_nodes.size (); ++p) {
            // If the point is clustered we pass over it
            // if not - we make it a new center
            if (clustered[p]) {
                continue;
            }
            Point new_center (center_nodes[p]->value ().x (), center_nodes[p]->value ().y (), 0);
            vector<bool> in_circle (center_nodes.size (), false);
            // the vector shows if the point is close enough to center
            for (int i = 0; i < center_nodes.size (); ++i) {
                Point p_i (center_nodes[i]->value ().x (), center_nodes[i]->value ().y (), 0);
                double distance = Point::dist (&new_center, &p_i);
                if (distance < R) {
                    in_circle[i] = true;
                }
            }
            bool changed = true;
            while (changed) {
                changed = false;

                // recomputing center
                new_center = Point ();
                int node_size = 0;
                for (int i = 0; i < center_nodes.size (); ++i) {
                    if (in_circle[i]) {
                        new_center = new_center + center_nodes[i]->value ();
                        node_size++;
                    }
                }
                new_center = new_center / node_size;

                // updating circles
                for (int i = 0; i < center_nodes.size (); ++i) {
                    Point p_i (center_nodes[i]->value ().x (), center_nodes[i]->value ().y (), 0);
                    double distance = Point::dist (&new_center, &p_i);
                    if (distance < R) {
                        if (!in_circle[i]) {
                            changed = true;
                            in_circle[i] = true;
                        }
                    } else {
                        if (in_circle[i]) {
                            changed = true;
                            in_circle[i] = false;
                        }
                    }
                }
                //frl_fprintf_e (print_number, center_nodes, new_center_nodes, clustered, in_circle, R, new_center);
                print_number++;
            }
            auto *node = new TreeNode<Point> (new_center);
            for (int i = 0; i < in_circle.size (); ++i) {
                if (in_circle[i] && !clustered[i]) {
                    node->add_child (center_nodes[i]); // the problem is here. I cant find the moment where i put brothers to the ring
                    clustered[i] = true;
                }
            }
            new_center_nodes.push_back (node);
        }
        center_nodes = new_center_nodes;
        R *= 2;
    }

}
