#ifndef CLUSTERANALYSES_CONTROLLER_H
#define CLUSTERANALYSES_CONTROLLER_H
#include "Field.h"
#include "Cluster_Search.h"
#include <iostream>
#include "Wave_algorithm.h"
#include "DBscan_algorithm.h"
class Controller {
private:
    string log_file_name;
    ofstream logger;
    bool write_log;
    void log (const string &s);
    Field *field_;
    //static Field *field_;
    vector <Cluster_Search *>  cluster_searches_;
    Wave_algorithm *waveAlgorithm;
    DBscan_algorithm *dbscanAlgorithm;
public:
    explicit Controller (bool write_log = true);
    ~Controller ();
    int get_help (int id, string &file_name);
    int generate_cloud (int id, double center_x, double center_y, double disp_x, double disp_y, int c_length);
    int fprintf (const string &file_name) const;
    void histogram (int pieces, vector<int> &x_distr, vector<int> &y_distr) const;
    int buffer_add_cloud (int id) const;
    int buffer_unload () const;
    int buffer_shift (double x, double y) const;
    int buffer_zoom (double k) const;
    int buffer_mirror () const;
    int buffer_rotate (double angle) const;
    int matrix () const;
    int incidence_matrix (double delta);
    int wave (int search_id);
    int dbscan (int search_id, int k);
    int k_means (int clusters_number);
    int em (int clusters_number);
    const vector<Cluster_Search> &info_cluster_search () const;
    vector<int> s_tree () const;
    int k_means_cores (int clusters_number, int cores_number);
    int hierarchical_algorithm ();
    int forel ();
    int delaunay ();
    int plot();
};


#endif //CLUSTERANALYSES_CONTROLLER_H
