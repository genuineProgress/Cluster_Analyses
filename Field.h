
#ifndef CLUSTERANALYSES_FIELD_H
#define CLUSTERANALYSES_FIELD_H

#include "Buffer.h"
#include "Cluster_Search.h"
#include "TreeNode.h"
class Cluster_Search;
class Field {
    friend class Wave_algorithm;
public:
    friend class Buffer;
    Buffer buf;
    Field ();
    ~Field ();
    int add (Cloud *addition);
    int fprintf (ofstream &out) const;
    int length () const;
    const vector<vector<double>> &dist () const;
    const vector<Cloud *> &cloud ();
    bool readonly () const;
    void create_dist_matrix ();
    const vector<Cluster_Search> &searches () const;
    void create_edges_matrix (double delta,vector <Cluster_Search *> &cluster_searches_);
    int db_clustering (int search_id, int k);
    int k_means (int clusters_number);
    vector<int> s_tree ();
    int k_means_cores (int clusters_number, int cores_number);
    int em (int clusters_number);
    int hieararchical_algorithm ();
    int forel ();
    int delaunay ();
    int add (const vector<Point> &addition);
    int plot();
private:
    Field &operator= (Field const &f);
    Field (Field const &f);
    vector<Cloud *> cloud_;
    vector<vector<double>> dist_; // matrix of distances
    vector<Cluster_Search> searches_;
    bool readonly_=false;
    int stree_add_closest_point (TreeNode<int> &node, vector<bool> &added);
    void stree_get_closest (TreeNode<int> &current,
                            vector<bool> &added,
                            double &min_dist,
                            TreeNode<int> *&tree_node,
                            TreeNode<int> *&out_node);
    vector<int> stree_create_histogram (TreeNode<int> &root, int pieces);
    void stree_picking_histogram (TreeNode<int> *node, double min_dist, double max_dist, vector<int> &histogram);
    void stree_get_tree_range (TreeNode<int> *&node, double &min_dist, double &max_dist);
    void stree_put_value_to_histogram (vector<int> &histogram, double min_dist, double max_dist, double distance);
    void stree_fprintf_tree (const string &file_name, TreeNode<int> *root);
    void stree_fprintf_node (TreeNode<int> *node, ofstream *out);
    static void fprintf_incidence_graph (const Cluster_Search &search, int id);
    vector<int> delaunay_get_viewable_points (vector<int> points_sorted, int i, vector<vector<bool>> connections);
    void delaunay_fix (int a, int b, int c, int d, vector<vector<bool>> connections);
    int sgn_vec_product (Point vec_1, Point vec_2);

    struct Edge {
        const Point *a, *b;

        Edge (const Point *a, const Point *b) : a (a), b (b) {
        };
    };

    static void delaunay_base_run (vector<Edge> &baselines,
                                   vector<vector<bool>> &edge,
                                   vector<bool> &points_done,
                                   int iteration);

    static double delaunay_angle_to_edge (Edge &edge, int num);
    static void delaunay_fprintf (vector<vector<bool>> &edge, int iteration);
};

#endif //CLUSTERANALYSES_FIELD_H
