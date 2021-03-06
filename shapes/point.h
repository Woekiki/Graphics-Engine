#ifndef POINT_H_
#define POINT_H_

namespace shp
{
    class Point
    {
    public:
        Point();
        Point(double x, double y);

        const double get_x();
        const double get_y();

        const unsigned int get_pixel_x();
        const unsigned int get_pixel_y();

    private:
        const double x;
        const double y;
    };
}

#endif