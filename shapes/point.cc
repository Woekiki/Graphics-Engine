#define _USE_MATH_DEFINES
#include "point.h"
#include "math.h"

shp::Point::Point()
    : Point(0, 0)
{
}

shp::Point::Point(double x, double y)
    : _x(x), _y(y), _z(0.0)
{
}

shp::Point::Point(double x, double y, double z) : _x(x), _y(y), _z(z) {}

shp::Point::Point(const shp::Point &other) : _x(other._x), _y(other._y), _z(other._z)
{
}

shp::Point &shp::Point::operator=(const shp::Point &other)
{
    _x = other._x;
    _y = other._y;
    _z = other._z;
    return (*this);
}

const double shp::Point::get_x()
{
    return _x;
}

const double shp::Point::get_y()
{
    return _y;
}

const double shp::Point::get_z()
{
    return _z;
}

void shp::Point::set_x(double x)
{
    _x = x;
}

void shp::Point::set_y(double y)
{
    _y = y;
}

void shp::Point::set_z(double z)
{
    _z = z;
}

const unsigned int shp::Point::get_pixel_x()
{
    return ceil(_x);
}

const unsigned int shp::Point::get_pixel_y()
{
    return ceil(_y);
}

const unsigned int shp::Point::get_pixel_z()
{
    return ceil(_z);
}

void shp::Point::translate(double a, double b, double c)
{
    _x += a;
    _y += b;
    _z += c;
}

void shp::Point::rotate_z_axis(double radians)
{
    _x = _x * cos(radians) + _y * sin(radians);
    _y = -_x * sin(radians) + _y * cos(radians);
}

void shp::Point::rotate_x_axis(double radians)
{
    _y = _y * cos(radians) + _z * sin(radians);
    _z = -_y * sin(radians) + _z * cos(radians);
}

void shp::Point::rotate_y_axis(double radians)
{
    _x = _x * cos(radians) - _z * sin(radians);
    _z = _x * sin(radians) + _z * cos(radians);
}

// double shp::Point::get_polar_r()
// {
//     return sqrt(pow(_x, 2) + pow(_y, 2) + pow(_z, 2));
// }

// double shp::Point::get_polar_theta()
// {
//     return acos(_z / get_polar_r());
// }

// double shp::Point::get_polar_phi()
// {
//     return atan(_y / _x);
// }

void shp::Point::transform_to_eye_coordinate(shp::PolarPoint eye)
{
    // double r = eye.get_polar_r();
    // double theta = eye.get_polar_theta();
    // double phi = eye.get_polar_phi();

    rotate_z_axis(eye.get_theta() + M_PI / 2.0);
    rotate_x_axis(eye.get_phi());
    translate(0, 0, -eye.get_r());
}

shp::Point shp::Point::project()
{
    return shp::Point(-_x / _z, -_y / _z);
}