#ifndef POINT_H_
#define POINT_H_

#include "polar_point.h"

namespace shp
{
    class Point
    {
    public:
        Point();
        Point(double x, double y);
        Point(double x, double y, double z);
        Point(const Point &other);

        Point &operator=(const Point &other);

        void translate(double a, double b, double c);
        void rotate_z_axis(double radians);
        void rotate_y_axis(double radians);
        void rotate_x_axis(double radians);

        const double get_x();
        const double get_y();
        const double get_z();

        void set_x(double x);
        void set_y(double y);
        void set_z(double z);

        const unsigned int get_pixel_x();
        const unsigned int get_pixel_y();
        const unsigned int get_pixel_z();

        // double get_polar_r();
        // double get_polar_phi();
        // double get_polar_theta();

        void transform_to_eye_coordinate(shp::PolarPoint eye);

        shp::Point project();

    private:
        double _x;
        double _y;
        double _z;
    };
}

#endif