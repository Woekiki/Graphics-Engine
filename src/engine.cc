#define _USE_MATH_DEFINES
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <set>
#include <map>
#include "ini_configuration.hh"
#include "EasyImage.h"
#include "color_rectangle.h"
#include "shapes/point.h"
#include "shapes/polar_point.h"
#include "shapes/line.h"
#include "renderer.h"
#include "shapes/l_system.h"
#include "math.h"
#include "assert.h"

const std::string AUTHOR = "Stijn Janssens";
const std::string VERSION = "0.0.3";
const img::Color BACKGROUND_COLOR = img::Color(242, 255, 204);
const std::string OUTPUT_FILE = "result_image.bmp";

double degrees_to_radians(double radians)
{
  return radians * M_PI / 180.0;
}

void startup_engine()
{
  const std::string SEPARATOR = "=======================";
  std::cout << std::endl
            << SEPARATOR << std::endl;
  std::cout << "Graphics engine" << std::endl;
  std::cout << "Code by: " << AUTHOR << std::endl;
  std::cout << "Version: " << VERSION << std::endl
            << SEPARATOR << std::endl
            << std::endl;
}

void parse_configuration(ini::Configuration &configuration, std::string filename)
{
  std::cout << "Reading config file " << filename << std::endl;

  std::ifstream input_configuration(filename);
  input_configuration >>
      configuration;
  input_configuration.close();

  return;
}

std::vector<std::pair<unsigned int, unsigned int>> generate_lines_from_faces(std::vector<std::vector<unsigned int>> faces)
{
  std::vector<std::pair<unsigned int, unsigned int>> lines;
  for (std::vector<unsigned int> face : faces)
  {
    for (unsigned int i = 0; i < face.size() - 1; i++)
    {
      lines.push_back({face[i] - 1, face[i + 1] - 1});
    }
    lines.push_back({face[face.size() - 1] - 1, face[0] - 1});
  }
  return lines;
}

std::pair<std::vector<shp::Point>, std::vector<std::vector<unsigned int>>> split_triangles(std::vector<shp::Point> input_points, std::vector<std::vector<unsigned int>> input_faces)
{
  std::vector<shp::Point> points;
  std::vector<std::vector<unsigned int>> faces;

  // TODO Optimalisation possible if we check if the newly generated points already exist.
  for (std::vector<unsigned int> face : input_faces)
  {
    shp::Point a = input_points[face[0] - 1];
    shp::Point b = input_points[face[1] - 1];
    shp::Point c = input_points[face[2] - 1];

    // TODO Use operator overloading to sum points and divide by a constant.
    shp::Point d = shp::Point((a.get_x() + b.get_x()) / 2.0, (a.get_y() + b.get_y()) / 2.0, (a.get_z() + b.get_z()) / 2.0);
    shp::Point e = shp::Point((a.get_x() + c.get_x()) / 2.0, (a.get_y() + c.get_y()) / 2.0, (a.get_z() + c.get_z()) / 2.0);
    shp::Point f = shp::Point((b.get_x() + c.get_x()) / 2.0, (b.get_y() + c.get_y()) / 2.0, (b.get_z() + c.get_z()) / 2.0);

    unsigned int current_index = points.size() + 1;

    points.push_back(a);
    points.push_back(b);
    points.push_back(c);
    points.push_back(d);
    points.push_back(e);
    points.push_back(f);

    faces.push_back({current_index, current_index + 3, current_index + 4});
    faces.push_back({current_index + 1, current_index + 5, current_index + 3});
    faces.push_back({current_index + 2, current_index + 4, current_index + 5});
    faces.push_back({current_index + 3, current_index + 5, current_index + 4});
  }

  return {points, faces};
}

std::vector<shp::Point> rescale_sphere(std::vector<shp::Point> input_points)
{
  std::vector<shp::Point> points(input_points.size(), shp::Point());
  for (unsigned int i = 0; i < input_points.size(); i++)
  {
    shp::Point orig_point = input_points[i];
    double divider = sqrt(pow(orig_point.get_x(), 2) + pow(orig_point.get_y(), 2) + pow(orig_point.get_z(), 2));
    points[i] = shp::Point(orig_point.get_x() / divider, orig_point.get_y() / divider, orig_point.get_z() / divider);
  }
  return points;
}

img::EasyImage render_l_system(ini::Configuration &configuration)
{
  int size = configuration["ImageProperties"]["size"].as_int_or_die();
  std::string initiator = configuration["ImageProperties"]["initiator"].as_string_or_die();
  int n = configuration["ImageProperties"]["n"].as_int_or_die();
  double alpha = degrees_to_radians(configuration["ImageProperties"]["alpha"].as_double_or_die());
  double delta = degrees_to_radians(configuration["ImageProperties"]["delta"].as_double_or_die());

  std::set<char> _alphabet = {};
  std::string alphabet = configuration["ImageProperties"]["alphabet"].as_string_or_die();
  assert(alphabet[0] == '{' && alphabet[alphabet.size() - 1] == '}');
  for (char c : alphabet.substr(1, alphabet.size() - 2))
  {
    if (c != ',')
    {
      _alphabet.insert(c);
    }
  }

  std::map<char, std::string> _replacements = {};
  std::string replacements = configuration["ImageProperties"]["replacement"].as_string_or_die();
  assert(replacements[0] == '{' && replacements[replacements.size() - 1] == '}');
  char replacement_left;
  std::string replacement_str = "";
  bool parse_left = true;
  for (char c : replacements.substr(1, replacements.size()))
  {
    if (c == ',' || c == '}')
    {
      _replacements.insert({replacement_left, replacement_str});
      parse_left = true;
      replacement_str = "";
      continue;
    }
    if (parse_left)
    {
      replacement_left = c;
      parse_left = false;
    }
    else
    {
      if (c != ':')
      {
        replacement_str += c;
      }
    }
  }

  std::cout << "Rendering L system" << std::endl;

  shp::L_System l_system = shp::L_System(_alphabet, initiator, alpha, delta, _replacements);

  std::pair<std::vector<shp::Point>, std::vector<std::pair<unsigned int, unsigned int>>> result = l_system.generate(n);
  std::vector<shp::Point> points = result.first;
  std::vector<std::pair<unsigned int, unsigned int>> lines = result.second;
  rndr::Renderer renderer = rndr::Renderer(points, lines);
  img::EasyImage image = renderer.render_2D(size, img::Color(173, 216, 230), img::Color(34, 139, 34));
  return image;
}

img::EasyImage render_wireframe(ini::Configuration &configuration)
{
  int size = configuration["ImageProperties"]["size"].as_int_or_die();
  std::string type = configuration["ImageProperties"]["type"].as_string_or_die();
  std::vector<shp::Point> _points;
  std::vector<std::vector<unsigned int>> _faces = {};
  if (type == "Cube")
  {
    _points = {shp::Point(1, -1, -1), shp::Point(-1, 1, -1), shp::Point(1, 1, 1), shp::Point(-1, -1, 1), shp::Point(1, 1, -1), shp::Point(-1, -1, -1), shp::Point(1, -1, 1), shp::Point(-1, 1, 1)};
    _faces = {{1, 5, 3, 7}, {5, 2, 8, 3}, {2, 6, 4, 8}, {6, 1, 7, 4}, {7, 3, 8, 4}, {1, 6, 2, 5}};
  }
  else
  {
    if (type == "Tetrahedron")
    {
      _points = {shp::Point(1, -1, -1), shp::Point(-1, 1, -1), shp::Point(1, 1, 1), shp::Point(-1, -1, 1), shp::Point(1, 1, -1), shp::Point(-1, -1, -1), shp::Point(1, -1, 1), shp::Point(-1, 1, 1)};
      _faces = {{1, 2, 3}, {2, 4, 3}, {1, 4, 2}, {1, 3, 4}};
    }
    else
    {
      if (type == "Octahedron")
      {
        _points = {shp::Point(1, 0, 0), shp::Point(0, 1, 0), shp::Point(-1, 0, 0), shp::Point(0, -1, 0), shp::Point(0, 0, -1), shp::Point(0, 0, 1)};
        _faces = {{1, 2, 6}, {2, 3, 6}, {3, 4, 6}, {4, 1, 6}, {2, 1, 5}, {3, 2, 5}, {4, 3, 5}, {1, 4, 5}};
      }
      else
      {
        if (type == "Icosahedron" || type == "Dodecahedron" || type == "Sphere")
        {
          _points = {shp::Point(0, 0, sqrt(5) / 2.0)};
          for (unsigned int i = 2; i < 7; i++)
          {
            _points.push_back(shp::Point(cos((i - 2) * 2 * (M_PI / 5.0)), sin((i - 2) * 2 * (M_PI / 5.0)), 0.5));
          }
          for (unsigned int i = 7; i < 12; i++)
          {
            _points.push_back(shp::Point(cos(M_PI / 5.0 + (i - 7) * 2 * M_PI / 5.0), sin(M_PI / 5.0 + (i - 7) * 2 * M_PI / 5.0), -0.5));
          }
          _points.push_back(shp::Point(0, 0, -sqrt(5) / 2.0));

          _faces = {{1, 2, 3},
                    {1, 3, 4},
                    {1, 4, 5},
                    {1, 5, 6},
                    {1, 6, 2},
                    {2, 7, 3},
                    {3, 7, 8},
                    {3, 8, 4},
                    {4, 8, 9},
                    {4, 9, 5},
                    {5, 9, 10},
                    {5, 10, 6},
                    {6, 10, 11},
                    {6, 11, 2},
                    {2, 11, 7},
                    {12, 8, 7},
                    {12, 9, 8},
                    {12, 10, 9},
                    {12, 11, 10},
                    {12, 7, 11}};
          if (type == "Sphere")
          {
            unsigned int n = configuration["ImageProperties"]["n"].as_int_or_die();

            for (unsigned int i = 0; i < n; i++)
            {
              std::pair<std::vector<shp::Point>, std::vector<std::vector<unsigned int>>> split_result = split_triangles(_points, _faces);
              _points = split_result.first;
              _faces = split_result.second;
            }
            _points = rescale_sphere(_points);
          }
          else
          {
            if (type == "Dodecahedron")
            {
              std::vector<shp::Point> icosahedron_points = _points;
              _points.clear();

              for (std::vector<unsigned int> face : _faces)
              {
                double x = 0;
                double y = 0;
                double z = 0;
                for (unsigned int triangle_point_index : face)
                {
                  x += icosahedron_points[triangle_point_index - 1].get_x();
                  y += icosahedron_points[triangle_point_index - 1].get_y();
                  z += icosahedron_points[triangle_point_index - 1].get_z();
                }
                x /= face.size();
                y /= face.size();
                z /= face.size();
                _points.push_back(shp::Point(x, y, z));
              }
              _faces = std::vector<std::vector<unsigned int>>({{1, 2, 3, 4, 5}, {1, 6, 7, 8, 2}, {2, 8, 9, 10, 3}, {3, 10, 11, 12, 4}, {4, 12, 13, 14, 5}, {5, 14, 15, 6, 1}, {20, 19, 18, 17, 16}, {20, 15, 14, 13, 19}, {19, 13, 12, 11, 18}, {18, 11, 10, 9, 17}, {17, 9, 8, 7, 16}, {16, 7, 6, 15, 20}});
            }
          }
        }
      }
    }
  }
  std::vector<std::pair<unsigned int, unsigned int>> _lines = generate_lines_from_faces(_faces);
  rndr::Renderer renderer = rndr::Renderer(_points, _lines);

  double r = configuration["Eye"]["r"].as_double_or_die();
  double theta = degrees_to_radians(configuration["Eye"]["theta"].as_double_or_die());
  double phi = degrees_to_radians(configuration["Eye"]["phi"].as_double_or_die());

  shp::PolarPoint eye = shp::PolarPoint(r, phi, theta);
  img::EasyImage image = renderer.render_3D(size, img::Color(173, 216, 230), img::Color(34, 0, 34), eye);
  return image;
}

void save_image(const img::EasyImage &image)
{
  std::ofstream image_output(OUTPUT_FILE);
  image_output << image;
  image_output.close();
}

int main(int argc, char const *argv[])
{
  startup_engine();
  if (argc < 2)
  {
    throw std::invalid_argument("Please provide a configuration file");
  }
  ini::Configuration configuration;

  parse_configuration(configuration, argv[1]);

  if (configuration["General"]["type"].as_string_or_die() == "IntroColorRectangle")
  {
    // ColorRectangle::colorRectangle(complete_image);
  }

  // TEST CODE FOR LINE DRAWING

  // shp::Point point1(1, 1);
  // shp::Point point2(12, 5);
  // shp::Point point3(15, 7);
  // shp::Point point4(28, 2);
  // shp::Point point5(25, 5);
  // shp::Point point6(29, 14);
  // shp::Point point7(7, 5);
  // shp::Point point8(2, 13);

  // std::vector<shp::Point> points = std::vector<shp::Point>({point1, point2, point3, point4, point5, point6, point7, point8});

  // std::vector<std::pair<unsigned int, unsigned int>> lines = {{0, 1}, {2, 3}, {4, 5}, {6, 7}};

  img::EasyImage complete_image;
  if (configuration["General"]["type"].as_string_or_die() == "LSystem")
  {
    complete_image = render_l_system(configuration);
  }
  else
  {
    if (configuration["General"]["type"].as_string_or_die() == "Wireframe")
    {
      complete_image = render_wireframe(configuration);
    }
  }

  // rndr::Renderer renderer = rndr::Renderer(points, lines, img_width, img_height);
  // img::EasyImage complete_image = renderer.render(img_width, BACKGROUND_COLOR, img::Color(178, 0, 0));
  std::cout << "Saving image: " << complete_image.get_width() << ", " << complete_image.get_height() << std::endl;
  save_image(complete_image);
}