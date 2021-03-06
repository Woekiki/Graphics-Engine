#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include "ini_configuration.hh"
#include "EasyImage.h"
#include "color_rectangle.h"
#include "shapes/point.h"
#include "shapes/line.h"
#include "renderer.h"

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

  int img_height = configuration["ImageProperties"]["height"];
  int img_width = configuration["ImageProperties"]["width"];

  img::EasyImage complete_image = img::EasyImage(img_width, img_height, BACKGROUND_COLOR);

  if (configuration["General"]["type"].as_string_or_die() == "IntroColorRectangle")
  {
    // ColorRectangle::colorRectangle(complete_image);
  }

  shp::Point point1(1, 1);
  shp::Point point2(12, 5);
  shp::Point point3(15, 7);
  shp::Point point4(28, 2);
  shp::Point point5(25, 5);
  shp::Point point6(28, 14);
  shp::Point point7(8, 5);
  shp::Point point8(2, 13);

  std::vector<shp::Point> points = std::vector<shp::Point>({point1, point2, point3, point4, point5, point6, point7, point8});

  std::vector<std::pair<unsigned int, unsigned int>> lines = {{0, 1}, {2, 3}, {4, 5}, {6, 7}};

  // For some reason 0,0,0 and 256,256,256 is the same here ??
  // Of course, colors work modulo 256. So the color white would be 255
  // complete_image(0, 0) = img::Color(255, 255, 255);

  rndr::Renderer renderer = rndr::Renderer(points, lines, img_width, img_height);

  // TODO DEBUG although the lines are calculated they do not show up on this image :(
  renderer.render(complete_image, img::Color(178, 0, 0));

  save_image(complete_image);
}