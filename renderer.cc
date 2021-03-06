#include "renderer.h"
#include "shapes/line.h"

rndr::Renderer::Renderer(){};

rndr::Renderer::Renderer(std::vector<shp::Point> points,
                         std::vector<std::pair<unsigned int, unsigned int>> lines,
                         unsigned int width,
                         unsigned int height)
    : _points(points), _lines(lines), _width(width), _height(height){};

void rndr::Renderer::render(img::EasyImage &image, const img::Color &color)
{
    for (std::pair<unsigned int, unsigned int> line_spec : this->_lines)
    {
        shp::Point &startPoint = this->_points.at(line_spec.first);
        shp::Point &endPoint = this->_points.at(line_spec.second);

        shp::Line line = shp::Line(startPoint, endPoint);
        line.render(image, color);
    }
}