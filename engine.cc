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
#include "shapes/line.h"
#include "renderer.h"
#include "shapes/l_system.h"
#include "math.h"
#include "assert.h"

const std::string AUTHOR = "Stijn Janssens";
const std::string VERSION = "0.0.3";
const img::Color BACKGROUND_COLOR = img::Color(242, 255, 204);
const std::string OUTPUT_FILE = "result_image.bmp";

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

img::EasyImage render_l_system(ini::Configuration &configuration)
{
  int size = configuration["ImageProperties"]["size"].as_int_or_die();
  std::string initiator = configuration["ImageProperties"]["initiator"].as_string_or_die();
  int n = configuration["ImageProperties"]["n"].as_int_or_die();
  double alpha = configuration["ImageProperties"]["alpha"].as_double_or_die() * M_PI / 180.0;
  double delta = configuration["ImageProperties"]["delta"].as_double_or_die() * M_PI / 180.0;

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
  std::cout << "size: " << size << " n: " << n << " alpha: " << alpha << " delta: " << delta << " initiator: " << initiator << " alphabet: " << std::endl;

  shp::L_System l_system = shp::L_System(_alphabet, initiator, alpha, delta, _replacements);

  std::pair<std::vector<shp::Point>, std::vector<std::pair<unsigned int, unsigned int>>> result = l_system.generate(n);
  std::vector<shp::Point> points = result.first;
  std::vector<std::pair<unsigned int, unsigned int>> lines = result.second;
  std::cout << "Points:  " << points[0].get_x() << ", " << points[0].get_y();
  std::cout << "Lines: " << lines[0].first << ", " << lines[0].second;
  rndr::Renderer renderer = rndr::Renderer(points, lines);
  img::EasyImage image = renderer.render(size, img::Color(173, 216, 230), img::Color(34, 139, 34));
  std::cout << "Saving image: " << image.get_width() << ", " << image.get_height() << std::endl;
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

  // img::EasyImage complete_image = img::EasyImage(img_width, img_height, BACKGROUND_COLOR);

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

  // rndr::Renderer renderer = rndr::Renderer(points, lines, img_width, img_height);
  // img::EasyImage complete_image = renderer.render(img_width, BACKGROUND_COLOR, img::Color(178, 0, 0));
  std::cout << "Saving image: " << complete_image.get_width() << ", " << complete_image.get_height() << std::endl;
  save_image(complete_image);
}