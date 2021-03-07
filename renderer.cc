#include "renderer.h"
#include "shapes/line.h"

rndr::Renderer::Renderer(){};

rndr::Renderer::Renderer(std::vector<shp::Point> points, std::vector<std::pair<unsigned int, unsigned int>> lines) : _points(points), _lines(lines), _width(0), _height(0){};

rndr::Renderer::Renderer(std::vector<shp::Point> points,
                         std::vector<std::pair<unsigned int, unsigned int>> lines,
                         unsigned int width,
                         unsigned int height)
    : _points(points), _lines(lines), _width(width), _height(height){};

img::EasyImage rndr::Renderer::render(unsigned int size, const ::img::Color &background_color, const img::Color &color)
{
    this->scale(size);
    img::EasyImage image(_width, _height, background_color);
    this->render(image, color);
    std::cout << "Rendered image " << image.get_width() << ", " << image.get_height() << std::endl;
    return image;
}

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

void rndr::Renderer::scale(unsigned int size)
{
    double x_min = INT16_MAX;
    double y_min = INT16_MAX;
    double x_max = 0;
    double y_max = 0;
    for (shp::Point point : _points)
    {
        double current_x = point.get_x();
        if (current_x > x_max)
        {
            x_max = current_x;
        }
        if (current_x < x_min)
        {
            x_min = current_x;
        }
        double current_y = point.get_y();
        if (current_y > y_max)
        {
            y_max = current_y;
        }
        if (current_y < y_min)
        {
            y_min = current_y;
        }
    }

    double x_range = x_max - x_min;
    double y_range = y_max - y_min;
    double bottom = std::max(x_range, y_range);

    _width = size * double(x_range / bottom);
    _height = 1 + size * double(y_range / bottom);

    double scale_factor = MARGIN * _width / x_range;
    for (shp::Point &point : _points)
    {
        double scaled_x = point.get_x() * scale_factor;
        double scaled_y = point.get_y() * scale_factor;

        double center_data_x = scale_factor * (x_min + x_max) / 2.0;
        double center_data_y = scale_factor * (y_min + y_max) / 2.0;

        double moved_x = scaled_x + (_width / 2.0 - center_data_x);
        double moved_y = scaled_y + (_height / 2.0 - center_data_y);

        // std::cout << "Scaled point: " << moved_x << ", " << moved_y << std::endl;

        point.set_x(moved_x);
        point.set_y(moved_y);
    }
}