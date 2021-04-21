#ifndef CLUSTERANALYSES_CLUSTER_SEARCH_H
#define CLUSTERANALYSES_CLUSTER_SEARCH_H

#include "Cloud.h"
#include "TreeNode.h"
#include "Wave_algorithm.h"
#define EM_LIMITATION 0.01

class Field;

class Cluster_Search {
    friend class Wave_algorithm;
public:
    class Cluster {
    public:
        explicit Cluster (vector<const Point *> vec);
        explicit Cluster (const vector<int> &vec);
        vector<const Point *> points; //It's never read, but really works to save the points
    };

    explicit Cluster_Search (Field *field, double delta = 0, int k = 0);
    Cluster_Search (Cluster_Search const &cs);
    Cluster_Search &operator= (Cluster_Search const &cs);
    const vector<vector<bool>> &edges () const;
    double delta; // max distance between points considered to have an edge
    int k; // 0 if not dbscan; in dbscan - density of cluster
    vector<Cluster> clusters;
    void add (const Cluster &addition);
    void create_edges_matrix ();
    vector<int> db_sorting (int density);
    void db_clustering (const vector<int> &state);
    const TreeNode<const Point *> *tree_root ();
    Cluster_Search dbscan (int density);
    Cluster_Search k_means (int clusters_number);
    Cluster_Search k_means_cores (int clusters_number, int cores_number);
    Cluster_Search hierarchical_algorithm ();
    Cluster_Search forel ();
    Cluster_Search em (int clusters_number);
private:
    Field *field_;
    vector<vector<bool>> edges_;
    static void dbscan_fprintf_type (const vector<int> &state);
    void kmeans_fprintf (vector<int> &nearest_cluster, vector<Point> &cores, int iteration);
    static void kmeans_core_fprintf (const vector<int> &nearest_cluster,
                                     const vector<vector<Point>> &cores,
                                     int iteration);
    void ha_get_closest_nodes (int &a, int &b, const vector<TreeNode<const Point *> *> &tree_node);
    void ha_merge_nodes (int a, int b, vector<TreeNode<const Point *> *> &tree_nodes);
    Point *ha_get_new_node_center (TreeNode<const Point *> *&first, TreeNode<const Point *> *&second);
    void ha_get_node_sum (TreeNode<const Point *> *&node, double &sum_x, double &sum_y, int &points);
    static void ha_fprintf (const vector<TreeNode<const Point *> *> &tree_nodes,
                            int iteration,
                            const Point &old_1,
                            const Point &old_2,
                            ofstream &tree);
    TreeNode<const Point *> *tree_root_;
    TreeNode<const Point *> *&ha_get_node_by_coords (int a,
                                                     vector<TreeNode<const Point *> *> &tree_node,
                                                     int &i,
                                                     double x,
                                                     double y);
    static void frl_fprintf (int print_number,
                             const vector<TreeNode<struct Point> *> &center_nodes,
                             const vector<TreeNode<struct Point> *> &new_center_nodes,
                             vector<bool> clustered,
                             vector<bool> in_circle,
                             double R,
                             const Point& circle_center);
    void frl_fprintf_e (int print_num,
                        vector<TreeNode<Point> *> center_nodes,
                        vector<TreeNode<Point> *> new_centers_node,
                        vector<bool> clustered,
                        vector<bool> in_circle,
                        double R,
                        Point center);
    double N (const Point *a, vector<double> m, vector<double> Sgm);
    static void em_fprintf (int iteration, vector<vector<double>> sgm, vector<vector<double>> m, vector<vector<double>> r, int clusters_number);
    void ha_fprintf_tree (const TreeNode<const Point *> *tree_node, ofstream& out);
};


#endif //CLUSTERANALYSES_CLUSTER_SEARCH_H
