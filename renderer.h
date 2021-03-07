#ifndef RENDERER_H
#define RENDERER_H

#include "shapes/point.h"
#include <vector>
#include "EasyImage.h"

namespace rndr
{
    class Renderer
    {
    public:
        Renderer();
        Renderer(std::vector<shp::Point> points, std::vector<std::pair<unsigned int, unsigned int>> lines);
        Renderer(std::vector<shp::Point> points, std::vector<std::pair<unsigned int, unsigned int>> lines, unsigned int width, unsigned int height);

        void scale(unsigned int size);
        void render(img::EasyImage &image, const img::Color &color);
        img::EasyImage render(unsigned int size, const ::img::Color &background_color, const img::Color &color);

    private:
        std::vector<shp::Point> _points;
        std::vector<std::pair<unsigned int, unsigned int>> _lines; // 2 indexes to points list, for start and end point
        unsigned int _width;
        unsigned int _height;
        const double MARGIN = 0.9;
    };
}

#endif