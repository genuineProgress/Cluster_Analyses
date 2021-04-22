//
// Created by 1 on 22.04.2021.
//

#include "EM_algorithm.h"
#define EPS 0.01
double EM_algorithm::N (const Point *a, vector<double> m, vector<double> Sgm) {
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

int EM_algorithm::process(vector<Cluster_Search *> cluster_searches_, Field *field, int clusters_number) {
    /* Errors
        * -1 field is not in readonly mode
        */
    if (!field->readonly ()) {
        return -1;
    }
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
        //em_fprintf (iteration, sigma, mu, r, clusters_number);
    } while (!sw_em);

}
