#include "color_rectangle.h"

void ColorRectangle::colorRectangle(img::EasyImage &image)
{
    for (unsigned int i = 0; i < image.get_width(); i++)
    {
        for (unsigned int j = 0; j < image.get_height(); j++)
        {
            image(i, j).red = i;
            image(i, j).green = j;
            image(i, j).blue = (i + j) % 256;
        }
    }
    return;
}