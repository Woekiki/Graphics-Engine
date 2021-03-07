#ifndef LINE_H_
#define LINE_H_
#include "point.h"
#include "../EasyImage.h"

namespace shp
{
    class Line
    {
    public:
        Line(shp::Point start, shp::Point end);

        void render(img::EasyImage &image, const img::Color &color);

    private:
        shp::Point startPoint;
        shp::Point endPoint;
    };
}

#endif