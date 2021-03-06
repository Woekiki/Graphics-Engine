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

    shp::Point leftPoint, rightPoint;
    if (this->startPoint.get_pixel_x() < this->endPoint.get_pixel_x())
    {
        leftPoint = this->startPoint;
        rightPoint = this->endPoint;
    }
    else
    {
        leftPoint = this->endPoint;
        rightPoint = this->startPoint;
    }

    unsigned int left_x = leftPoint.get_pixel_x();
    unsigned int left_y = leftPoint.get_pixel_y();
    unsigned int right_x = rightPoint.get_pixel_x();
    unsigned int right_y = rightPoint.get_pixel_y();

    const double top = double(right_y) - double(left_y);
    const double bottom = double(right_x) - double(left_x);
    const double direction_coefficient = top / bottom;

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