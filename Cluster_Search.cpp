#include "Cluster_Search.h"
#include "Field.h"
#include <utility>
#define EPS 0.01

Cluster_Search::Cluster::Cluster (vector<const Point *> vec) : points (vec) {
}

Cluster_Search::Cluster::Cluster (const vector<int> &vec) {
    vector<const Point *> a;
    a.reserve (vec.size ());
    for (int i : vec) {
        a.push_back (Point::get_by_id (i + 1));
    }
    points = a;
}

const vector<vector<bool>> &Cluster_Search::edges () const {
    return edges_;
}

void Cluster_Search::add (const Cluster_Search::Cluster &addition) {
    clusters.push_back (addition);
}

Cluster_Search::Cluster_Search (Field *field, double delta, int k) : field_ (field), delta (delta), k (k) {
    if (delta != 0) {
        // a little defence for kmeans
        create_edges_matrix ();
    }
}

void Cluster_Search::create_edges_matrix () {
    // matrix of incidences
    if (edges_.empty ()) {
        vector<vector<bool>> incidence (Point::quantity (), vector<bool> (Point::quantity (), false));
        for (int i = 0; i < Point::quantity (); ++i) {
            for (int j = i + 1; j < Point::quantity (); ++j) {
                incidence[j][i] = incidence[i][j] = (field_->dist ()[i][j] < delta);
            }
        }
        edges_ = incidence;
    }
}


vector<int> Cluster_Search::db_sorting (int density) {
    // counts number of neighbours for all the points
    k = density;
    vector<int> type (Point::quantity (), 0);
    for (int i = 0; i < Point::quantity (); ++i) {
        vector<int> neighbours;
        for (int j = 0; j < Point::quantity (); ++j) {
            if (edges ()[i][j]) {
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
    return type;
}

void Cluster_Search::db_clustering (const vector<int> &state) {
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
                        if (!burnt[j] && i != j && edges ()[i][j]) {
                            new_added.push_back (j);
                            burnt[j] = true;
                        }
                    }
                }
                curr_cluster.insert (curr_cluster.end (), new_added.begin (), new_added.end ());
                curr_added = new_added;
                new_added.clear ();
            }
            clusters.emplace_back (curr_cluster);
        } else if (state[l] == 0) {
            // Doesn't seem to be true.
            // That point can be connected to core point which has id number more than ours
            clusters.emplace_back (vector<int> (1, l));
            burnt[l] = true;
        }
    }
    dbscan_fprintf_type (state);
    //fprintf_clusters();
}

Cluster_Search Cluster_Search::dbscan (int density) {
    db_clustering (db_sorting (density));
    return *this;
}

Cluster_Search::Cluster_Search (const Cluster_Search &cs)
        : delta (cs.delta), k (cs.k), field_ (cs.field_), clusters (cs.clusters), edges_ (cs.edges_) {
}

Cluster_Search &Cluster_Search::operator= (const Cluster_Search &cs) {
    if (this != &cs) {
        delta = cs.delta;
        k = cs.k;
        field_ = cs.field_;
        clusters = cs.clusters;
        edges_ = cs.edges_;
    }
    return *this;
}

void Cluster_Search::dbscan_fprintf_type (const vector<int> &state) {
    ofstream core ("gnuplot/dbscan/core.txt");
    ofstream peripherical ("gnuplot/dbscan/peripherical.txt");
    for (int i = 0; i < state.size (); ++i) {
        if (state[i] == 2) {
            core << Point::get_by_id (i + 1)->x () << " " << Point::get_by_id (i + 1)->y () << endl;
        } else if (state[i] == 1) {
            peripherical << Point::get_by_id (i + 1)->x () << " " << Point::get_by_id (i + 1)->y () << endl;
        }
    }
}

Cluster_Search Cluster_Search::k_means (int clusters_number) {
    bool changed = true;
    vector<Point> cores (clusters_number); // Pointers to core points
    vector<int> nearest_cluster (Point::quantity (), 0); // shows which element of cores is the nearest to that point
    // filling core vector by random points
    for (int i = 0; i < cores.size (); ++i) {
        const Point *t = Point::get_by_id (i + 1);
        cores[i] = Point (t->x (), t->y (), 0);
    }
    int iteration = 0;
    while (changed || iteration == 1) {
        // searching the closest point for each field point
        changed = false;
        for (int i = 0; i < Point::quantity (); ++i) {
            for (int j = 0; j < cores.size (); ++j) {
                double a = Point::dist (Point::get_by_id (i + 1), &cores[j]);
                double b = Point::dist (Point::get_by_id (i + 1), &cores[nearest_cluster[i]]);
                if (a < b) {
                    changed = true;
                    nearest_cluster[i] = j;
                }
            }
        }
        // recomputing cores
        vector<int> cluster_length (clusters_number, 0);
        for (auto &core : cores) {
            core = Point ();
        }
        for (int i = 0; i < Point::quantity (); ++i) {
            cores[nearest_cluster[i]].shift (*Point::get_by_id (i + 1));
            cluster_length[nearest_cluster[i]]++;
        }
        for (int i = 0; i < cores.size (); ++i) {
            cores[i] = Point (cores[i].x () / cluster_length[i],
                              cores[i].y () / cluster_length[i],
                              0);
        }
        kmeans_fprintf (nearest_cluster, cores, iteration);
        iteration++;
    }
    clusters.clear ();
    vector<vector<int>> clusters_to_set (clusters_number);
    for (int i = 0; i < nearest_cluster.size (); ++i) {
        clusters_to_set[nearest_cluster[i]].push_back (i);
    }
    for (int i = 0; i < clusters_number; ++i) {
        clusters.emplace_back (clusters_to_set[i]);
    }
    return *this;
}

void Cluster_Search::kmeans_fprintf (vector<int> &nearest_cluster, vector<Point> &cores, int iteration) {
    ofstream out ("gnuplot/kmeans/km" + to_string (iteration) + ".txt");
    for (int i = 0; i < Point::quantity (); ++i) {
        out << Point::get_by_id (i + 1)->x () << " " << Point::get_by_id (i + 1)->y () << " " << nearest_cluster[i]
            << endl;
    }
    for (auto &core : cores) {
        out << core.x () << " " << core.y () << " " << -1
            << endl;
    }
}

Cluster_Search Cluster_Search::k_means_cores (int clusters_number, int cores_number) {
    /* realises k-means withcores clustering
     * clusters_number shows how many clusters we'll get
     * cores_number shows how many cores each cluster contains
     */
    vector<int> nearest_cluster (Point::quantity (), 0);
    // shows which cluster's core is the nearest to that point.
    // values are from 0 to clusters_number-1
    vector<int> nearest_core (Point::quantity (), 0);
    // shows which core in cluster is the closest.
    // Values are from 0 to cores_number-1
    vector<vector<Point>> cores (clusters_number, vector<Point> (cores_number)); // Cores of clusters itself
    // filling core vector by random points
    for (int i = 0; i < cores.size (); ++i) {
        for (int j = 0; j < cores[i].size (); ++j) {
            const Point *t = Point::get_by_id (i * cores.size () + j + 1);
            cores[i][j] = Point (t->x (), t->y (), 0);
        }
    }
    // iterations of k-means
    int iteration = 0;
    bool changed = true;
    while (changed) {
        // searching the closest core point for each field point
        changed = false;
        for (int i = 0; i < Point::quantity (); ++i) {
            for (int j = 0; j < cores.size (); ++j) {
                for (int l = 0; l < cores[j].size (); ++l) {
                    double a = Point::dist (Point::get_by_id (i + 1), &cores[j][l]);
                    double b = Point::dist (Point::get_by_id (i + 1), &cores[nearest_cluster[i]][nearest_core[i]]);
                    if (a < b) {
                        changed = true;
                        nearest_cluster[i] = j;
                        nearest_core[i] = l;
                    }
                }
            }
        }
        // recomputing cores
        // vector<vector<int>> cluster_length_ (clusters_number, vector<int> (cores_number, 0));
        for (auto &cluster : cores) {
            for (auto &core : cluster) {
                core = Point ();
            }
        }
        // here we just collect points we need before the action
        // the first step is running kmeans for each cluster
        for (int i = 0; i < cores.size (); i++) {
            auto &cluster = cores[i];
            // searching which points current cluster contains
            vector<int> curr_points;
            // throws double free or corruption if number of clusters is not equal to number of cores.
            // maybe i'm messing up that number while counting length of vectors
            // another hypotheses says that something wrong with Point::shift function
            for (int p = 0; p < Point::quantity (); ++p) {
                if (nearest_cluster[p] == i) {
                    curr_points.push_back (p);
                }
            }
            for (int j = 0; j < cluster.size (); ++j) {
                const Point *t = Point::get_by_id (curr_points[j] + 1);
                cluster[j] = Point (t->x (), t->y (), 0);
            }
            bool changed_in = true;
            int iteration_in = 0;
            while (changed_in) {
                changed_in = false;
                for (int curr_point : curr_points) {
                    for (int j = 0; j < cluster.size (); ++j) {
                        double a = Point::dist (Point::get_by_id (curr_point + 1), &cluster[j]);
                        double b = Point::dist (Point::get_by_id (curr_point + 1), &cluster[nearest_core[curr_point]]);
                        if (a < b) {
                            changed_in = true;
                            nearest_core[curr_point] = j;
                        }
                    }
                }
                // recomputing cores
                vector<int> subcluster_length (cores_number, 0);
                for (auto &l : cluster) {
                    l = Point ();
                }
                for (int curr_point : curr_points) {
                    cluster[nearest_core[curr_point]].shift (*Point::get_by_id (curr_point + 1));
                    subcluster_length[nearest_core[curr_point]]++;
                }
                for (int l = 0; l < cluster.size (); ++l) {
                    cluster[l] = Point (cluster[l].x () / subcluster_length[l],
                                        cluster[l].y () / subcluster_length[l],
                                        0);
                }
                iteration_in++;
            }
        }
        kmeans_core_fprintf (nearest_cluster, cores, iteration);
        iteration++;
    }
    clusters.clear ();
    vector<vector<int>> clusters_to_set (clusters_number);
    for (int i = 0; i < nearest_cluster.size (); ++i) {
        clusters_to_set[nearest_cluster[i]].push_back (i);
    }
    for (int i = 0; i < clusters_number; ++i) {
        clusters.emplace_back (clusters_to_set[i]);
    }
    return *this;
}

void Cluster_Search::kmeans_core_fprintf (const vector<int> &nearest_cluster,
                                          const vector<vector<Point>> &cores,
                                          int iteration) {
    ofstream out ("gnuplot/kmcores/km" + to_string (iteration) + ".txt");
    for (int i = 0; i < Point::quantity (); ++i) {
        out << Point::get_by_id (i + 1)->x () << " " << Point::get_by_id (i + 1)->y () << " " << nearest_cluster[i]
            << endl;
    }
    for (auto &cluster : cores) {
        for (auto &core:cluster) {
            out << core.x () << " " << core.y () << " " << -1 << endl;
        }
    }
}

Cluster_Search Cluster_Search::hierarchical_algorithm () {
    int tree_number = Point::quantity ();
    vector<TreeNode<const Point *> *> tree_nodes;
    tree_nodes.reserve (Point::quantity ());
    ofstream tree ("gnuplot/ha/ha_tree.txt");
    for (int p = 0; p < Point::quantity (); ++p) {
        tree_nodes.push_back (new TreeNode<const Point *> (Point::get_by_id (p + 1)));
    }
    while (1 < tree_nodes.size ()) {
        int a, b;
        ha_get_closest_nodes (a, b, tree_nodes);
        Point old_1 (tree_nodes[a]->value ()->x (), tree_nodes[a]->value ()->y (), 0);
        Point old_2 (tree_nodes[b]->value ()->x (), tree_nodes[b]->value ()->y (), 0);
        ha_merge_nodes (a, b, tree_nodes);
        ha_fprintf (tree_nodes, Point::quantity () - tree_nodes.size (), old_1, old_2, tree);
    }
    tree_root_ = tree_nodes[0];
    return *this;
}

void Cluster_Search::ha_get_closest_nodes (int &a, int &b, const vector<TreeNode<const Point *> *> &tree_node) {
    // searches for closest nodes writes its numbers to a and b
    double min_dist = field_->dist ()[0][1];
    int min_a = 0;
    int min_b = 1;
    for (a = 0; a < tree_node.size (); ++a) {
        for (b = a + 1; b < tree_node.size (); ++b) {
            double distance = Point::dist (tree_node[a]->value (), tree_node[b]->value ());
            if (distance < min_dist) {
                min_dist = distance;
                min_a = a;
                min_b = b;
            }
        }
    }
    a = min_a;
    b = min_b;
}

void Cluster_Search::ha_merge_nodes (int a, int b, vector<TreeNode<const Point *> *> &tree_nodes) {

    /* there are that steps:
     * get center of new node
     * connect old centers to new node
     * delete old pointers from vector tree_node
     * add new node to that vector
     */

    int i_1, i_2;
    TreeNode<const Point *> *&first = tree_nodes[a];
    TreeNode<const Point *> *&second = tree_nodes[b];
    Point *new_center = ha_get_new_node_center (first, second);
    auto *new_node = new TreeNode<const Point *> (new_center);
    /* int min_i = min (i_1, i_2);
    int max_i = max (i_1, i_2); */
    int min_i = min (a, b);
    int max_i = max (a, b);
    tree_nodes.erase (tree_nodes.begin () + max_i);
    tree_nodes.erase (tree_nodes.begin () + min_i);
    tree_nodes.push_back (new_node);
}

TreeNode<const Point *> *&Cluster_Search::ha_get_node_by_coords (int a,
                                                                 vector<TreeNode<const Point *> *> &tree_node,
                                                                 int &i, double x, double y) {
    for (i = 0; i < tree_node.size (); ++i) {
        if (tree_node[i]->value ()->x () == x && tree_node[i]->value ()->y () == y) {
            return tree_node[i];
        }
    }
    // just to escape warning, it's impossible branch
    return tree_node[0];
}

Point *Cluster_Search::ha_get_new_node_center (TreeNode<const Point *> *&first, TreeNode<const Point *> *&second) {
    double sum_x = 0;
    double sum_y = 0;
    int points = 0;
    ha_get_node_sum (first, sum_x, sum_y, points);
    ha_get_node_sum (second, sum_x, sum_y, points);
    return new Point (sum_x / points, sum_y / points, 0);
}

void Cluster_Search::ha_get_node_sum (TreeNode<const Point *> *&node, double &sum_x, double &sum_y, int &points) {
    if (node->first_child () != nullptr) {
        ha_get_node_sum (node->first_child_, sum_x, sum_y, points);
    }
    if (node->brother () != nullptr) {
        ha_get_node_sum (node->brother_, sum_x, sum_y, points);
    }
    sum_x += node->value ()->x ();
    sum_y += node->value ()->y ();
    points++;
}

void Cluster_Search::ha_fprintf (const vector<TreeNode<const Point *> *> &tree_nodes,
                                 int iteration,
                                 const Point &old_1,
                                 const Point &old_2,
                                 ofstream &tree) {
    if (tree_nodes.size () < 3) {
        double a;
    }
    ofstream out ("gnuplot/ha/ha" + to_string (iteration) + ".txt");
    for (auto tree_node : tree_nodes) {
        out << tree_node->value ()->x () << " " << tree_node->value ()->y () << " " << 0 << endl;
    }
    out << old_1.x () << " " << old_1.y () << " " << 1 << endl;
    out << old_2.x () << " " << old_2.y () << " " << 1 << endl;
    tree << old_1.x() << " " << old_1.y() << endl;
    tree << tree_nodes.back()->value()->x() << " " << tree_nodes.back()->value()->y() << endl << endl;
    tree << old_2.x() << " " << old_2.y() << endl;
    tree << tree_nodes.back()->value()->x() << " " << tree_nodes.back()->value()->y() << endl << endl;
}

const TreeNode<const Point *> *Cluster_Search::tree_root () {
    return tree_root_;
}

Cluster_Search Cluster_Search::forel () {
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
                frl_fprintf_e (print_number, center_nodes, new_center_nodes, clustered, in_circle, R, new_center);
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
    return *this;
}

void Cluster_Search::frl_fprintf (int print_number,
                                  const vector<TreeNode<struct Point> *> &center_nodes,
                                  const vector<TreeNode<struct Point> *> &new_center_nodes,
                                  vector<bool> clustered,
                                  vector<bool> in_circle,
                                  double R,
                                  const Point &circle_center) {
    ofstream out ("gnuplot/forel/frl" + to_string (print_number) + ".txt");
    for (int i = 0; i < center_nodes.size (); ++i) {
        if (!clustered[i] && !in_circle[i]) {
            out << center_nodes[i]->value ().x () << " " << center_nodes[i]->value ().y () << " -2" << endl;
        }
    }
    for (int i = 0; i < new_center_nodes.size (); ++i) {
        out << new_center_nodes[i]->value ().x () << " " << new_center_nodes[i]->value ().y () << " -1" << endl;
        if (new_center_nodes[i]->first_child () != nullptr) {

            auto *pointer = new_center_nodes[i]->first_child ();
            do {
                out << pointer->value ().x () << " " << pointer->value ().y () << " " << i << endl;
                if (pointer->brother () == nullptr) {
                    break;
                } else {
                    pointer = pointer->brother ();
                }
            } while (true);
        }
    }
    for (int i = 0; i < in_circle.size (); ++i) {
        out << center_nodes[i]->value ().x () << " " << center_nodes[i]->value ().y () << " -3" << endl;
    }
    ofstream circle ("forel/circle" + to_string (print_number) + ".txt");
    circle << circle_center.x () << " " << circle_center.y () << " " << R << endl;
}

void Cluster_Search::frl_fprintf_e (int print_num,
                                    vector<TreeNode<Point> *> center_nodes, vector<TreeNode<Point> *> new_centers_node,
                                    vector<bool> clustered,
                                    vector<bool> in_circle,
                                    double R,
                                    Point center) {
    ofstream out ("gnuplot/forel/frl" + to_string (print_num) + ".txt");
    ofstream circle ("gnuplot/forel/circle" + to_string (print_num) + ".txt");
    for (int i = 0; i < new_centers_node.size (); ++i) {
        circle << new_centers_node[i]->value ().x () << " " << new_centers_node[i]->value ().y () << " " << R << endl;
        auto *pointer = new_centers_node[i]->first_child ();
        while (pointer != nullptr) {
            out << pointer->value ().x () << " " << pointer->value ().y () << " " << i << endl;
            pointer = pointer->brother ();
        }
    }
    // current circle print
    for (int i = 0; i < in_circle.size (); ++i) {
        if (in_circle[i]) {
            out << center_nodes[i]->value ().x () << " " << center_nodes[i]->value ().y () << " -1" << endl;
        }
    }
    circle << center.x () << " " << center.y () << " " << R << endl;
    //printing unclustered points
    for (int i = 0; i < center_nodes.size (); ++i) {
        if (!clustered[i] && !in_circle[i]) {
            out << center_nodes[i]->value ().x () << " " << center_nodes[i]->value ().y () << " " << endl;
        }
    }
}

double Cluster_Search::N (const Point *a, vector<double> m, vector<double> Sgm) {
    double det = Sgm[0] * Sgm[3] - Sgm[1] * Sgm[2];
    vector<vector<double>> b_s (2, vector<double> (2));
    b_s[0][0] = Sgm[3] / det;
    b_s[0][1] = -Sgm[1] / det;
    b_s[1][0] = -Sgm[2] / det;
    b_s[1][1] = Sgm[0] / det;
    if (det < 0) { det = -det; }
    return exp (-(b_s[0][0] * (a->x () - m[0]) * (a->x () - m[0])
                  + (b_s[1][0] + b_s[0][1]) * (a->x () - m[0]) * (a->y () - m[1])
                  + b_s[1][1] * (a->y () - m[1]) * (a->y () - m[1])) / 2) / (sqrt (2 * M_PI * det));
}

Cluster_Search Cluster_Search::em (int clusters_number) {
    vector<vector<double>> sigma;
    vector<vector<double>> mu;
    vector<vector<double>> r;
    vector<double> pi;
    vector<double> v0;
    // filling mu sigma and pi vectors
    for (int i = 0; i < clusters_number; ++i) {
        vector<vector<double> > v22;
        vector<double> v4;
        vector<double> v2;
        for (int j = 0; j < 4; j++) {
            v4.push_back (0);
        }
        for (int j = 0; j < 2; j++) {
            v2.push_back (0);
        };
        sigma.push_back (v4);
        mu.push_back (v2);
        // mu looks like to be randomly set
        mu[i][0] = Point::get_by_id (i + 1)->x ();
        mu[i][1] = Point::get_by_id (i + 1)->y ();
        pi.push_back ((double) 1 / clusters_number);
        v0.push_back (0);
        //possibly here should be created all the clusters
    }
    for (int i = 0; i < Point::quantity (); ++i) {
        r.push_back (v0);
    }
    for (int i = 0; i < clusters_number; ++i) {
        vector<vector<double>> a (2, vector<double> (2, 0));
        for (int j = 0; j < Point::quantity (); ++j) {
            a[0][0] += (Point::get_by_id (j + 1)->x () - mu[i][0]) * (Point::get_by_id (j + 1)->x () - mu[i][0]);
            a[0][1] += (Point::get_by_id (j + 1)->x () - mu[i][0]) * (Point::get_by_id (j + 1)->y () - mu[i][1]);
            a[1][0] += (Point::get_by_id (j + 1)->y () - mu[i][1]) * (Point::get_by_id (j + 1)->x () - mu[i][0]);
            a[1][1] += (Point::get_by_id (j + 1)->y () - mu[i][1]) * (Point::get_by_id (j + 1)->y () - mu[i][1]);
        }
        sigma[i][0] = a[0][0] / Point::quantity ();
        sigma[i][1] = a[0][1] / Point::quantity ();
        sigma[i][2] = a[1][0] / Point::quantity ();
        sigma[i][3] = a[1][1] / Point::quantity ();
    }
    bool sw_em;
    int iteration = 0;
    do {
        iteration++;
        sw_em = true;

        // E step
        // I have no idea what all that stuff do
        double s = 0;
        vector<double> sum;
        for (int i = 0; i < Point::quantity (); ++i) {
            s = 0;
            for (int c = 0; c < clusters_number; ++c) {
                s += pi[c] * N (Point::get_by_id (i + 1), mu[c], sigma[c]);
            }
            sum.push_back (s);
        }
        for (int i = 0; i < Point::quantity (); ++i) {
            for (int c = 0; c < clusters_number; ++c) {
                if (((r[i][c] - pi[c] * N (Point::get_by_id (i + 1), mu[c], sigma[c]) / sum[i]) > EPS)
                    || ((r[i][c] - pi[c] * N (Point::get_by_id (i + 1), mu[c], sigma[c]) / sum[i]) < -EPS)) {
                    sw_em = false;
                }
                r[i][c] = pi[c] * N (Point::get_by_id (i + 1), mu[c], sigma[c]) / sum[i];
            }
        }

        // M step
        double m_c;
        for (int c = 0; c < clusters_number; ++c) {
            m_c = 0;
            mu[c][0] = 0;
            mu[c][1] = 0;
            for (int i = 0; i < Point::quantity (); ++i) {
                m_c += r[i][c];
            }
            for (int i = 0; i < Point::quantity (); ++i) {
                mu[c][0] += Point::get_by_id (i + 1)->x () * r[i][c] / m_c;
                mu[c][1] += Point::get_by_id (i + 1)->y () * r[i][c] / m_c;
            }
            vector<vector<double>> a (2, vector<double> (2, 0));
            for (int i = 0; i < Point::quantity (); ++i) {
                a[0][0] += r[i][c] * (Point::get_by_id (i + 1)->x () - mu[c][0])
                           * (Point::get_by_id (i + 1)->x () - mu[c][0]);
                a[0][1] += r[i][c] * (Point::get_by_id (i + 1)->x () - mu[c][0])
                           * (Point::get_by_id (i + 1)->y () - mu[c][1]);
                a[1][0] += r[i][c] * (Point::get_by_id (i + 1)->y () - mu[c][1])
                           * (Point::get_by_id (i + 1)->x () - mu[c][0]);
                a[1][1] += r[i][c] * (Point::get_by_id (i + 1)->y () - mu[c][1])
                           * (Point::get_by_id (i + 1)->y () - mu[c][1]);
            }
            sigma[c][0] = a[0][0] / m_c;
            sigma[c][1] = a[0][1] / m_c;
            sigma[c][2] = a[1][0] / m_c;
            sigma[c][3] = a[1][1] / m_c;
            pi[c] = m_c / Point::quantity ();
        }
        em_fprintf (iteration, sigma, mu, r, clusters_number);
    } while (!sw_em);
    // creating cluster
    return *this;
}

void Cluster_Search::em_fprintf (int iteration,
                                 vector<vector<double> > sgm,
                                 vector<vector<double> > m,
                                 vector<vector<double> > r,
                                 int clusters_number) {
    ofstream out ("gnuplot/em/em" + to_string (iteration) + ".txt");
    for (int p = 0; p < Point::quantity (); ++p) {
        int ind = 0;
        for (int c = 0; c < clusters_number; ++c) {
            if (r[p][c] > r[p][ind]) {
                ind = c;
            }
        }
        out << Point::get_by_id (p + 1)->x () << " " << Point::get_by_id (p + 1)->y () << " " << ind << endl;
    }
    ofstream ellipsis ("gnuplot/em/ellipse" + to_string (iteration) + ".txt");
    for (int c = 0; c < clusters_number; ++c) {
        double discr, angle; // discriminant
        vector<double> lambda;
        vector<double> v;
        discr = (sgm[c][0] + sgm[c][3]) * (sgm[c][0] + sgm[c][3]) - 4 * (sgm[c][0] * sgm[c][3] - sgm[c][1] * sgm[c][2]);
        if (discr >= 0) {
            lambda.push_back ((sgm[c][0] + sgm[c][3] + sqrt (discr)) / 2);
            lambda.push_back ((sgm[c][0] + sgm[c][3] - sqrt (discr)) / 2);
        }
        v.push_back (sgm[c][0] - lambda[0]);
        v.push_back (sgm[c][1]);
        if (v[1] < 0) {
            v[0] *= -1;
            v[1] *= -1;
        }
        angle = acos (v[0] / sqrt (v[0] * v[0] + v[1] * v[1]));
        angle *= 180;
        angle /= M_PI;
        ellipsis << m[c][0] << " " << m[c][1] << " " << 100 * lambda[0] << " " << 100 * lambda[1] << " " << angle
                 << endl;
    }
}

void Cluster_Search::ha_fprintf_tree (const TreeNode<const Point *> *tree_node, ofstream &out) {
    auto *pointer = tree_node->first_child ();
    while (pointer != nullptr) {
        out << tree_node->value ()->x () << " " << tree_node->value ()->y () << endl;
        out << pointer->value ()->x () << " " << pointer->value ()->y () << endl;
    }
    pointer = tree_node->first_child ();
    while (pointer != nullptr) {
        ha_fprintf_tree (pointer, out);
        pointer = pointer->brother ();
    }
}
