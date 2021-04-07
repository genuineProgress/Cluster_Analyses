#include "Point.h"
int Point::quantity_ = 0;

Point::Point () : id_ (0), x_ (0), y_ (0) {
}

Point::Point (double x, double y, int id) : x_ (x), y_ (y) {
    if (id != -1) {
        id_ = 0;
    } else {
        id_ = ++quantity_;
        if (id_pointers.empty ()) {
            id_pointers.push_back (nullptr);
        }
        id_pointers.push_back (this);
    }
}
/*
Point::Point (Point const &c) : x_ (c.x ()), y_ (c.y ()) {
    if (c.id () == 0) {
        id_ = 0;
    } else {
        id_ = ++quantity_;
        if (id_pointers.empty ()) {
            id_pointers.push_back (nullptr);
        }
        id_pointers.push_back (this);
    }
}*/

double Point::x () const {
    return x_;
}

double Point::y () const {
    return y_;
}

int Point::fprintf (ofstream &out) const {
    //prints all the clouds to file out
    out << x () << " " << y () << endl;
    return 0;
}

int Point::id () const {
    return id_;
}

void Point::shift (const double &x, const double &y) {
    x_ += x;
    y_ += y;
}

void Point::shift (const Point &vector) {
    //OFTEN THROWS INVALID WRITE IN VALGRIND
    x_ += vector.x ();
    y_ += vector.y ();
}

Point Point::operator+ (const Point &a) const {
    if (id () != 0 && a.id () != 0) {
        return Point (x () + a.x (), y () + a.y ());
    } else if (id () != 0 && a.id () == 0) {
        return Point (x () + a.x (), y () + a.y (), id ());
    } else if (a.id () != 0 && id () == 0) {
        return Point (x () + a.x (), y () + a.y (), a.id ());
    } else {
        return Point (x () + a.x (), y () + a.y (), 0);
    }
}

Point Point::operator* (const double &a) const {
    if (id () == 0) {
        return Point (x () * a, y () * a, 0);
    } else {
        return Point (x () * a, y () * a);
    }
}

Point Point::operator/ (const double &a) const {
    if (id () == 0) {
        return Point (x () / a, y () / a, 0);
    } else {
        return Point (x () / a, y () / a);
    }
}

const Point *Point::get_by_id (int id) {
    return id_pointers[id];
}

double Point::dist (const Point *A, const Point *B) {
    return sqrt (pow (A->x () - B->x (), 2) + pow (A->y () - B->y (), 2));
}

int Point::quantity () {
    return quantity_;
}

Point &Point::operator= (const Point &p) {
    if (this != &p) {
        x_ = p.x ();
        y_ = p.y ();
        if (p.id () != 0) {
            id_ = ++quantity_;
            if (id_pointers.empty ()) {
                id_pointers.push_back (nullptr);
            }
            id_pointers.push_back (this);
        }
    }
    return *this;
}

void Point::set (double x, double y) {
    x_ = x;
    y_ = y;
}

Point::Point (const Point &c, int id) : x_ (c.x ()), y_ (c.y ()) {
    if (id != -1 || c.id () == 0) {
        id_ = 0;
    } else {
        id_ = ++quantity_;
        if (id_pointers.empty ()) {
            id_pointers.push_back (nullptr);
        }
        id_pointers.push_back (this);
    }
}

vector<Point *> Point::id_pointers;