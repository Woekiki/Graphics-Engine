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

const double shp::Point::get_x()
{
    return this->x;
}

const double shp::Point::get_y()
{
    return this->y;
}

const unsigned int shp::Point::get_pixel_x()
{
    return round(this->x);
}

const unsigned int shp::Point::get_pixel_y()
{
    return round(this->y);
}