#include "l_system.h"
#define _USE_MATH_DEFINES
#include "math.h"
#include <iostream>

shp::L_System::L_System() : _alphabet({}), _initiator(""), _alpha(0), _delta(0), _replacements({}) {}
shp::L_System::L_System(std::set<char> alphabet,
                        std::string initiator,
                        double alpha,
                        double delta,
                        std::map<char, std::string> replacements) : _alphabet(alphabet), _initiator(initiator), _alpha(alpha), _delta(delta), _replacements(replacements) {}

std::string shp::L_System::_generate_string(unsigned int n)
{
    std::string current_string = _initiator;
    for (unsigned int i = 0; i < n; i++)
    {
        std::string new_string = "";
        for (char c : current_string)
        {
            if (c == '+' || c == '-' || c == '[' || c == ']')
            {
                new_string += c;
            }
            else
            {
                new_string += _replacements[c];
            }
        }
        current_string = new_string;
    }
    return current_string;
}

std::pair<std::vector<shp::Point>, std::vector<std::pair<unsigned int, unsigned int>>> shp::L_System::generate(unsigned int n)
{
    std::string full_string = _generate_string(n);
    //std::cout << "Full string: " << full_string << std::endl;

    shp::Point current_point = shp::Point(0.0, 0.0);
    std::vector<shp::Point> points = std::vector<shp::Point>({current_point});
    std::vector<std::pair<unsigned int, unsigned int>> lines;
    double current_alpha = _alpha;

    for (char current_char : full_string)
    {
        if (current_char == '+')
        {
            current_alpha = current_alpha + _delta;
            while (current_alpha > M_PI * 2.0)
            {
                current_alpha -= M_PI * 2.0;
            }
        }
        else
        {
            if (current_char == '-')
            {
                current_alpha = current_alpha - _delta;
                while (current_alpha < 0)
                {
                    current_alpha += M_PI * 2.0;
                }
            }
            else
            {
                // It is a move string
                shp::Point new_point = Point(current_point.get_x() + cos(current_alpha), current_point.get_y() + sin(current_alpha));
                points.push_back(new_point);
                current_point = new_point;
                // It is a 1 string
                if (_alphabet.find(current_char) != _alphabet.end())
                {
                    lines.push_back({points.size() - 2, points.size() - 1});
                }
            }
        }
    }

    // Printing
    // for (shp::Point point : points)
    // {
    //     std::cout << "Point: " << point.get_x() << ", " << point.get_y() << std::endl;
    // }

    // for (std::pair<unsigned int, unsigned int> line : lines)
    // {
    //     std::cout << "Line: " << line.first << ", " << line.second << std::endl;
    // }

    return {points, lines};
}
