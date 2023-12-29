#include <SFML/Graphics.hpp>
#include <fstream>
#include <iostream>
#include <memory>
#include <vector>

#include "imgui-SFML.h"
#include "imgui.h"

// TODO: Visualise rectangles and integerate IMGUI pannel

struct BoundingBox {
  float x_min, y_min, x_max, y_max;
};

class Circle {
 private:
  size_t p_id = 0;

 public:
  float radius;
  int segments = 32;
  std::string name;
  float pos_x;
  float pos_y;
  float delta_x;
  float delta_y;
  int color_r;
  int color_g;
  int color_b;
  bool alive;
  BoundingBox bounding_box;
  sf::CircleShape circle_shape;
  sf::Text text;
  Circle() {}
  Circle(std::string name_in, float pos_x_in, float pos_y_in, float delta_x_in,
         float delta_y_in, int color_r_in, int color_g_in, int color_b_in,
         float radius_in, size_t id)
      : name(name_in),
        pos_x(pos_x_in),
        pos_y(pos_y_in),
        delta_x(delta_x_in),
        delta_y(delta_y_in),
        color_r(color_r_in),
        color_g(color_g_in),
        color_b(color_b_in),
        radius(radius_in),
        p_id(id) {
    alive = true;
    circle_shape.setRadius(radius);
    circle_shape.setPointCount(segments);
    circle_shape.setPosition(pos_x, pos_y);
    circle_shape.setFillColor(sf::Color(color_r, color_g, color_b));
    text.setString(name);
  }
  void updatePosition(float window_width, float window_height) {
    pos_x = circle_shape.getPosition().x;
    pos_y = circle_shape.getPosition().y;

    updateBoundingBox();
    updateDelta(window_width, window_height);

    pos_x = pos_x + delta_x;
    pos_y = pos_y + delta_y;

    circle_shape.setPosition(pos_x, pos_y);
  }

  void updateDelta(float window_width, float window_height) {
    if (((bounding_box.x_max + delta_x) >= window_width) ||
        ((bounding_box.x_min + delta_x) <= 0.0f)) {
      delta_x = -delta_x;
    }

    if (((bounding_box.y_max + delta_y) >= window_height) ||
        ((bounding_box.y_min + delta_y) <= 0.0f)) {
      delta_y = -delta_y;
    }
  };
  void updateBoundingBox() {
    bounding_box.x_max = pos_x + 2 * radius;
    bounding_box.x_min = pos_x;
    bounding_box.y_max = pos_y + 2 * radius;
    bounding_box.y_min = pos_y;
  }
  void updateText(sf::Font& myFont) {
    text.setFont(myFont);

    auto textBounds = text.getLocalBounds();
    auto textHeight = (float)text.getCharacterSize();
    auto textX = pos_x + (bounding_box.x_max - bounding_box.x_min -
                          textBounds.width - textBounds.left) /
                             2;
    auto textY = pos_y + (bounding_box.y_max - bounding_box.y_min - textHeight -
                          textBounds.top) /
                             2;

    text.setPosition(textX, textY);
    auto textBoundsGloabl = text.getGlobalBounds();
  }
};

class Rectangle {
 private:
  size_t p_id = 0;

 public:
  float length;
  float breadth;
  std::string name;
  float pos_x;
  float pos_y;
  float delta_x;
  float delta_y;
  int color_r;
  int color_g;
  int color_b;
  bool alive;

  Rectangle() {}
  Rectangle(std::string name_in, float pos_x_in, float pos_y_in,
            float delta_x_in, float delta_y_in, int color_r_in, int color_g_in,
            int color_b_in, float length_in, float breadth_in, size_t id)
      : name(name_in),
        pos_x(pos_x_in),
        pos_y(pos_y_in),
        delta_x(delta_x_in),
        delta_y(delta_y_in),
        color_r(color_r_in),
        color_g(color_g_in),
        color_b(color_b_in),
        length(length_in),
        breadth(breadth_in),
        p_id(id) {
    alive = true;
  }
};

struct WindowParameters {
  int width;
  int height;
};

struct FontParameters {
  std::string fontPath;
  int size;
  int color_r;
  int color_g;
  int color_b;
};

typedef std::vector<std::shared_ptr<Rectangle>> RectangleVec;
typedef std::vector<std::shared_ptr<Circle>> CircleVec;

class EnityManager {
 public:
  WindowParameters window;
  FontParameters Font;
  RectangleVec m_rectanglesVec;
  CircleVec m_circlesVec;
  size_t p_id = 0;

  void loadFromFile(const std::string& filename) {
    std::ifstream fin(filename);

    std::string token;
    while (fin >> token) {
      if (token == "Window") {
        fin >> window.width;
        fin >> window.height;
      }
      if (token == "Font") {
        fin >> Font.fontPath;
        fin >> Font.size;
        fin >> Font.color_r;
        fin >> Font.color_g;
        fin >> Font.color_b;
      }

      if (token == "Circle") {
        std::string name;
        float pos_x;
        float pos_y;
        float delta_x;
        float delta_y;
        int color_r;
        int color_g;
        int color_b;
        float radius;
        fin >> name;
        fin >> pos_x;
        fin >> pos_y;
        fin >> delta_x;
        fin >> delta_y;
        fin >> color_r;
        fin >> color_g;
        fin >> color_b;
        fin >> radius;
        auto e =
            std::make_shared<Circle>(name, pos_x, pos_y, delta_x, delta_y,
                                     color_r, color_g, color_b, radius, p_id++);
        m_circlesVec.emplace_back(e);
      }

      if (token == "Rectangle") {
        std::string name;
        float pos_x;
        float pos_y;
        float delta_x;
        float delta_y;
        int color_r;
        int color_g;
        int color_b;
        float length;
        float breadth;
        fin >> name;
        fin >> pos_x;
        fin >> pos_y;
        fin >> delta_x;
        fin >> delta_y;
        fin >> color_r;
        fin >> color_g;
        fin >> color_b;
        fin >> length;
        fin >> breadth;
        auto e = std::make_shared<Rectangle>(name, pos_x, pos_y, delta_x,
                                             delta_y, color_r, color_g, color_b,
                                             length, breadth, p_id++);
        m_rectanglesVec.emplace_back(e);
      }
    }
  }
};

int main(int argc, char* argv[]) {
  EnityManager entity_manager;
  entity_manager.loadFromFile("../config/config.txt");

  std::cout << "Number of entities = " << entity_manager.p_id << "\n";

  auto wWidth = entity_manager.window.width;
  auto wHeight = entity_manager.window.height;

  sf::RenderWindow window(sf::VideoMode(wWidth, wHeight), "SFML Works!");
  window.setFramerateLimit(60);

  // initialize IMGUI and create a clock used for its timing
  ImGui::SFML::Init(window);
  sf::Clock deltaClock;

  // scale the imgui ui by a give factor, does not affect text size
  ImGui::GetStyle().ScaleAllSizes(1.0f);

  // // the imgui color {r, g, b} wheel requires floats from 0-1
  float c[3] = {0.0f, 1.0f, 1.0f};

  sf::Font myFont;

  // attempt to load the font from a file
  if (!myFont.loadFromFile(entity_manager.Font.fontPath)) {
    // if we can't load the font exit
    std::cerr << "Could not load font\n";
    exit(-1);
  }

  // main loop - continues for each frame, while window is open
  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      ImGui::SFML::ProcessEvent(window, event);

      // This event is treggered when window is closed
      if (event.type == sf::Event::Closed) {
        window.close();
      }
    }

    // // Loop through circles and update properties
    for (auto circle : entity_manager.m_circlesVec) {
      circle->updatePosition(window.getSize().x, window.getSize().y);
      circle->updateText(myFont);
    }
    // basic rendering function calls
    window.clear();

    for (auto circle : entity_manager.m_circlesVec) {
      if (circle->alive) {
        window.draw(circle->circle_shape);
        window.draw(circle->text);
      }
    }

    window.display();
  }
  return 0;
}