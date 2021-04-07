#include "Buffer.h"
void Buffer::add (Cloud *c) {
    //adds cloud to buffer
    for (int i = 0; i < c->length (); ++i) {
        //cloud.point_.push_back (new Point (*c->point ()[i]));
        points.emplace_back(*c->point ()[i], 0);
    }
}

void Buffer::add (Point *p) {
    // adds point to buffer
    // cloud.point_.push_back (new Point (*p));
    points.emplace_back(*p);
}

int Buffer::length () const {
    // returns number of points in buffer
    // return cloud.length ();
    return points.size ();
}

const vector<Point> &Buffer::unload () const {
    // returns all the points from buffer
    // return cloud.point ();
    return points;
}

void Buffer::shift (double shift_x, double shift_y) {
    //shifts the cloud to shift_x up and shift_y right
    int len = length ();
    for (int i = 0; i < len; ++i) {
        //cloud.point_[i]->shift (shift_x, shift_y);
        points[i].shift (shift_x, shift_y);
    }
}

void Buffer::rotate (double angle) {
    // rotates the cloud around the center counter-clockwise for a given angle
    double center_x = 0;
    double center_y = 0;
    /*
    for (const auto &p: cloud.point_) {
        center_x += p->x ();
        center_y += p->y ();
    }*/
    for (auto &point : points) {
        center_x += point.x ();
        center_y += point.y ();
    }
    center_x /= length ();
    center_y /= length ();
    double shift_x = -center_x;
    double shift_y = -center_y;
    shift (shift_x, shift_y);
    /*for (int i = 0; i < length (); ++i) {
        double old_x = cloud.point_[i]->x ();
        double old_y = cloud.point_[i]->y ();
        delete cloud.point_[i];
        cloud.point_[i] = new Point (old_x * cos (angle) - old_y * sin (angle),
                                     old_x * sin (angle) + old_y * cos (angle),
                                     0);
    }*/
    for (int i = 0; i < length (); ++i) {
        double old_x = points[i].x ();
        double old_y = points[i].y ();
        points[i] = Point (old_x * cos (angle) - old_y * sin (angle),
                           old_x * sin (angle) + old_y * cos (angle),
                           0);
    }
    shift (-shift_x, -shift_y);
}

void Buffer::mirror () {
    //makes a reflection about center and Oy
    double center_x = 0;
    /*for (const auto &p: cloud.point_) {
        center_x += p->x ();
    }*/
    for (const auto &p: points) {
        center_x += p.x ();
    }
    center_x /= length ();
    double shift_x = -center_x;
    shift (shift_x, 0);
    for (int i = 0; i < length (); ++i) {
        //cloud.point_[i]->shift (-cloud.point_[i]->x () * 2, 0);
        points[i].shift (-points[i].x () * 2, 0);
    }
    shift (-shift_x, 0);

}

void Buffer::zoom (double k) {
    // distance from the center to point increases in k times
    // the direction of vectors from the center to each point doesn't change
    double center_x = 0;
    double center_y = 0;
    //for (const auto &p: cloud.point_) {
    for (const auto &p: points) {
        center_x += p.x ();
        center_y += p.y ();
    }
    center_x /= length ();
    center_y /= length ();
    double shift_x = -center_x;
    double shift_y = -center_y;
    shift (shift_x, shift_y);
    for (int i = 0; i < length (); i++) {
        /*Point temp (cloud.point_[i]->x () * k, cloud.point_[i]->y () * k, 0);
        delete cloud.point_[i];
        cloud.point_[i] = new Point (cloud.point_[i]->x () * k, cloud.point_[i]->y () * k, 0);*/
        Point temp (points[i].x () * k, points[i].y () * k, 0);
        points[i] = Point (points[i].x () * k, points[i].y () * k, 0);
    }
    shift (-shift_x, -shift_y);
}

void Buffer::erase () {
    /*
    for (auto &p: cloud.point_) {
        delete p;
    }
    cloud.point_.clear ();*/
    points.clear ();
}
