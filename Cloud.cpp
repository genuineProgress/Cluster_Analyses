#include "Cloud.h"
#include <iostream>
// this thing is to be deleted
Cloud::Cloud () = default;

Cloud::Cloud (int length, double disp_x, double disp_y) {
    //creates the cloud with Gauss distribution
    //from length points with center in (0,0)
    //and radius 0.5
    //disp_x and disp_y - coefficients of Gauss distribution
    int a = time (nullptr);
    std::cout << a << endl;
    srand (a);
    //srand(1616779958);
    for (int i = 0; i < length; ++i) {
        double x, y;
        x = y = 0;
        for (int j = 0; j < M; ++j) {
            x += (double) rand () / RAND_MAX - 0.5;
        }
        x /= M;
        x *= disp_x;
        for (int j = 0; j < M; ++j) {
            y += (double) rand () / RAND_MAX - 0.5;
        }
        y /= M;
        y *= disp_y;
        point_.push_back (new Point (x, y));
    }
}

Cloud::Cloud (const vector<Point> &points) {
    for (const auto &point : points) {
        point_.push_back (new Point (point.x(), point.y()));
    }
}

Cloud &Cloud::operator= (Cloud const &c) {
    if (this != &c) {
        for (auto &p: point_) {
            delete p;
        }
        point_.clear ();
        for (int i = 0; i < c.length (); ++i) {
            point_.push_back (c.point_[i]);
        }
    }
    return *this;
}

Cloud::Cloud (Cloud const &c) {
    //Copy constructor
    for (auto &p: point_) {
        delete p;
    }
    point_.clear ();
    for (int i = 0; i < c.length (); ++i) {
        point_.push_back (c.point_[i]);
    }
}

Cloud::~Cloud () {
    for (auto &p: point_) {
        delete p;
    }
}

int Cloud::length () const {
    // returns the number of points in cloud
    return point_.size ();
}

void Cloud::shift (double shift_x, double shift_y) {
    //shifts the cloud to shift_x up and shift_y right
    int len = length ();
    for (int i = 0; i < len; ++i) {
        point_[i]->shift (shift_x, shift_y);
    }
}

int Cloud::fprintf (ofstream &out) const {
    //prints all the clouds to file out
    for (int i = 0; i < length (); ++i) {
        point_[i]->fprintf (out);
    }
    return 0;
}

const vector<Point *> &Cloud::point () const {
    return point_;
}
