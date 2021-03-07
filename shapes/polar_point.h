#ifndef POLAR_POINT_H
#define POLAR_POINT_H

namespace shp
{
    class PolarPoint
    {
    public:
        PolarPoint();
        PolarPoint(double r, double phi, double theta);

        double get_r();
        double get_phi();
        double get_theta();

    private:
        double _r;
        double _phi;
        double _theta;
    };
}

#endif