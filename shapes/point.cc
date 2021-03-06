#include "point.h"
#include "math.h"

shp::Point::Point()
    : Point(0, 0)
{
}

shp::Point::Point(double x, double y)
    : x(x), y(y)
{
}

shp::Point::Point(const shp::Point &other) : x(other.x), y(other.y)
{
}

shp::Point &shp::Point::operator=(const shp::Point &other)
{
    x = other.x;
    y = other.y;
    return (*this);
}

const double shp::Point::get_x()
{
    return this->x;
}

const double shp::Point::get_y()
{
    return this->y;
}

void shp::Point::set_x(double x)
{
    this->x = x;
}

void shp::Point::set_y(double y)
{
    this->y = y;
}

const unsigned int shp::Point::get_pixel_x()
{
    return ceil(this->x);
}

const unsigned int shp::Point::get_pixel_y()
{
    return ceil(this->y);
}