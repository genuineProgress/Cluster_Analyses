#ifndef CLUSTERANALYSES_POINT_H
#define CLUSTERANALYSES_POINT_H
#include <fstream>
#include <vector>
#include <cmath>
using namespace std;

/*
 * While using Point you need to know
 * - anywhere you can count current number of points using Point::quantity()
 * - the only way to make a new point on Field is to set id = -1 (by default constructor or manually)
 *   other way will lead you to loose point in Point::quantity and get_point_by_id
 * - If the point has id 0 it is service point.
 *   It is not counted on the field, can't be clustered and so on, but can be useful in your own operations
 * - You have arithmetical operators such as + Point, * double, /double. I hope their usage is obvious
 */

class Point {
    friend class Cloud;

private:
    double x_, y_; //coords
    int id_;
    int value_of_function=0;
    int fprintf (ofstream &out) const;
    static int quantity_;
    static vector<Point *> id_pointers; // vector of pointers because vector of references is not supported
public:
    Point ();
    //Point (Point const &c);
    Point (double x, double y, int id = -1);
    Point (Point const &c, int id = -1);
    Point &operator= (Point const &p);
    double x () const;
    double y () const;
    int id () const;
    void shift (const double &x, const double &y);
    void shift (const Point &vector);
    Point operator+ (const Point &a) const;
    Point operator* (const double &a) const;
    Point operator/ (const double &a) const;
    static const Point * get_by_id (int id);
    static double dist (const Point *A, const Point *B);
    static int quantity ();
    void set (double x, double y);
};

#endif //CLUSTERANALYSES_POINT_H
