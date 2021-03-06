#include "line.h"
#include "math.h"

shp::Line::Line(shp::Point start, shp::Point end) : startPoint(start), endPoint(end){};

void shp::Line::render(img::EasyImage &image, const img::Color &color)
{
    // VERTICAL LINE
    if (this->startPoint.get_pixel_x() == this->endPoint.get_pixel_x())
    {
        unsigned int x = this->startPoint.get_pixel_x();
        unsigned int min_y = std::min(this->startPoint.get_pixel_y(), this->endPoint.get_pixel_y());
        unsigned int max_y = std::max(this->startPoint.get_pixel_y(), this->endPoint.get_pixel_y());
        for (unsigned int i = min_y; i <= max_y; i++)
        {
            image(x, i) = color;
        }
        return;
    }
    // HORIZONTAL LINE
    if (this->startPoint.get_pixel_y() == this->endPoint.get_pixel_y())
    {
        unsigned int y = this->startPoint.get_pixel_y();
        unsigned int min_x = std::min(this->startPoint.get_pixel_x(), this->endPoint.get_pixel_x());
        unsigned int max_x = std::max(this->startPoint.get_pixel_x(), this->endPoint.get_pixel_x());
        for (unsigned int i = min_x; i <= max_x; i++)
        {
            image(i, y) = color;
        }
        return;
    }

    // TODO these 2 ifblocks work, but can be coded into one I think.
    unsigned int left_x, left_y, right_x, right_y;
    if (this->startPoint.get_pixel_x() < this->endPoint.get_pixel_x())
    {
        left_x = this->startPoint.get_pixel_x();
        left_y = this->startPoint.get_pixel_y();
        right_x = this->endPoint.get_pixel_x();
        right_y = this->endPoint.get_pixel_y();
    }
    else
    {
        left_x = this->endPoint.get_pixel_x();
        left_y = this->endPoint.get_pixel_y();
        right_x = this->startPoint.get_pixel_x();
        right_y = this->startPoint.get_pixel_y();
    }

    // std::cout << "Calculating: " << left_x << ", " << left_y << " - " << right_x << ", " << right_y << std::endl;

    const double top = double(right_y) - double(left_y);
    const double bottom = double(right_x) - double(left_x);
    const double direction_coefficient = top / bottom;

    // std::cout << "RICO: " << direction_coefficient << std::endl;

    if ((direction_coefficient <= 1.0 && direction_coefficient > 0.0) || (-1.0 <= direction_coefficient && direction_coefficient < 0.0))
    {
        for (unsigned int i = 0; i <= right_x - left_x; i++)
        {
            image(left_x + i, round(left_y + direction_coefficient * double(i))) = color;
        }
        return;
    }
    if (direction_coefficient > 1.0)
    {
        for (unsigned int i = 0; i <= right_y - left_y; i++)
        {
            // std::cout << round(left_x + i / direction_coefficient) << ", " << left_y + i << std::endl;
            // std::cout << i / direction_coefficient << std::endl;
            // std::cout << left_x + i / direction_coefficient << std::endl;
            image(round(left_x + i / direction_coefficient), left_y + i) = color;
        }
        return;
    }
    if (direction_coefficient < -1.0)
    {
        for (unsigned int i = 0; i <= left_y - right_y; i++)
        {
            image(round(left_x - i / direction_coefficient), left_y - i) = color;
        }
        return;
    }
}