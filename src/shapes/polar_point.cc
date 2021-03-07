#include "polar_point.h"

shp::PolarPoint::PolarPoint() : _r(0), _phi(0), _theta(0) {}

shp::PolarPoint::PolarPoint(double r, double phi, double theta) : _r(r), _phi(phi), _theta(theta) {}

double shp::PolarPoint::get_r()
{
    return _r;
}

double shp::PolarPoint::get_phi()
{
    return _phi;
}

double shp::PolarPoint::get_theta()
{
    return _theta;
}