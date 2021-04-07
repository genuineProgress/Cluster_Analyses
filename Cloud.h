#ifndef CLUSTERANALYSES_CLOUD_H
#define CLUSTERANALYSES_CLOUD_H

#include "Point.h"
#include <cmath>
const int M = 100;

class Cloud {
    Cloud (const vector<Point> &points);

    friend class Field;

    friend class Controller;

    friend class Buffer;

public:
    Cloud ();
    Cloud (int length, double disp_x, double disp_y);
    Cloud &operator= (Cloud const &c);
    Cloud (Cloud const &c);
    ~Cloud ();
    int length () const;
    void shift (double shift_x, double shift_y);
    const vector<Point *> &point () const;
private:
    int fprintf (ofstream &out) const;
    vector<Point *>
            point_; // vector of pointers used for having a unique point with unique id without problems while copying
};


#endif //CLUSTERANALYSES_CLOUD_H
