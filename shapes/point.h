#ifndef POINT_H_
#define POINT_H_

namespace shp
{
    class Point
    {
    public:
        Point();
        Point(double x, double y);
        Point(const Point &other);

        Point &operator=(const Point &other);

        const double get_x();
        const double get_y();

        void set_x(double x);
        void set_y(double y);

        const unsigned int get_pixel_x();
        const unsigned int get_pixel_y();

    private:
        double x;
        double y;
    };
}

#endif