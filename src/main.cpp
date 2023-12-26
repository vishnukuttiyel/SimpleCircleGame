#include <SFML/Graphics.hpp>
#include <fstream>
#include <iostream>
#include <memory>
#include <vector>

#include "imgui-SFML.h"
#include "imgui.h"

class Circle {
 private:
  float p_radius;
  size_t p_id = 0;

 public:
  std::string name;
  float pos_x;
  float pos_y;
  float delta_x;
  float delta_y;
  int color_r;
  int color_g;
  int color_b;
  bool alive;
  Circle() {}
  Circle(std::string name_in, float pos_x_in, float pos_y_in, float delta_x_in,
         float delta_y_in, int color_r_in, int color_g_in, int color_b_in,
         float p_radius_in, size_t id)
      : name(name_in),
        pos_x(pos_x_in),
        pos_y(pos_y_in),
        delta_x(delta_x_in),
        delta_y(delta_y_in),
        color_r(color_r_in),
        color_g(color_g_in),
        color_b(color_b_in),
        p_radius(p_radius_in),
        p_id(id) {
    alive = true;
  }
};

class Rectangle {
 private:
  float p_length;
  float p_breadth;
  size_t p_id = 0;

 public:
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
            int color_b_in, float length, float breadth, size_t id)
      : name(name_in),
        pos_x(pos_x_in),
        pos_y(pos_y_in),
        delta_x(delta_x_in),
        delta_y(delta_y_in),
        color_r(color_r_in),
        color_g(color_g_in),
        color_b(color_b_in),
        p_length(length),
        p_breadth(breadth),
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
  // create a wundow of size w*h pixels
  // top-left of the window is (0,0) and bottom-right is (w,h)
  // you will have to read these from the config file

  // const int wWidth = 1280;
  // const int wHeight = 720;

  // sf::RenderWindow window(sf::VideoMode(wWidth, wHeight), "SFML Works!");
  // window.setFramerateLimit(60);

  // // initialize IMGUI and create a clock used for its timing
  // ImGui::SFML::Init(window);
  // sf::Clock deltaClock;

  // // scale the imgui ui by a give factor, does not affect text size
  // ImGui::GetStyle().ScaleAllSizes(1.0f);

  // // the imgui color {r, g, b} wheel requires floats from 0-1
  // float c[3] = { 0.0f, 1.0f, 1.0f };

  // // shape
  // float circleRadius = 50;
  // int circleSegments = 32; // number of segments to draw the circle
  // float circleVelocityX = 1.0f;
  // float circleVelocityY = 0.5f;
  // bool drawCircle = true;
  // bool drawText = true;

  // // create the sfml circle shape based on our paramteres
  // sf::CircleShape circle(circleRadius, circleSegments);

  // sf::Font myFont;

  // // attempt to load the font from a file
  // if (!myFont.loadFromFile("fonts/tech.ttf")) {
  //   // if we can't load the font exit
  //   std::cerr << "Could not load font\n";
  //   exit(-1);
  // }

  // sf::Text text("Sample Text", myFont, 24);

  // text.setPosition(0, wHeight - (float)text.getCharacterSize());

  // // set up a charcter array to set the text
  // char displayString[255] = "Sample Text";

  // // main loop - continues for each frame, while window is open
  // while (window.isOpen()) {

  //   sf::Event event;
  //   while (window.pollEvent(event)) {
  //     ImGui::SFML::ProcessEvent(window, event);

  //     // This event is treggered when window is closed
  //     if (event.type == sf::Event::Closed) {
  //       window.close();
  //     }

  //     // this event is triggered when any key is pressed
  //     if (event.type == sf::Event::KeyPressed) {

  //       // print the key that was pressed to the console
  //       std::cout << " Key pressed with code =" << event.key.code << "\n";

  //       if (event.key.code == sf::Keyboard::X) {

  //         // reverse the x direction of the circle on the screen
  //         circleVelocityX *= -1.0f;
  //       }
  //     }
  //   }

  //   // update imgui of this frame with the time that the last frame took
  //   ImGui::SFML::Update(window, deltaClock.restart());

  //   ImGui::Begin("Window title");
  //   ImGui::Text("Window text!");
  //   ImGui::Checkbox("Draw Circle", &drawCircle);
  //   ImGui::SameLine();
  //   ImGui::Checkbox("Draw Text", &drawText);
  //   ImGui::SameLine();
  //   ImGui::SliderFloat("Radius", &circleRadius, 0.0f, 300.0f);
  //   ImGui::SliderInt("Sides", &circleSegments, 3, 64);
  //   ImGui::ColorEdit3("Color Circle", c);
  //   ImGui::InputText("Text", displayString, 255);

  //   if (ImGui::Button("Set Text")) {
  //     text.setString(displayString);
  //   }
  //   ImGui::SameLine();

  //   if (ImGui::Button("Reset Circle")) {
  //     circle.setPosition(0, 0);
  //   }
  //   ImGui::End();

  //   // set the circle properies because they may have been updated with the
  //   ui circle.setFillColor(sf::Color(c[0] * 255, c[1] * 255, c[2] * 255));
  //   circle.setPointCount(circleSegments);
  //   circle.setRadius(circleRadius);

  //   // basic animation - move in each frame
  //   circle.setPosition(circle.getPosition().x + circleVelocityX,
  //                      circle.getPosition().y + circleVelocityY);

  //   // basic rendering function calls
  //   window.clear();
  //   if (drawCircle) {

  //     window.draw(circle);
  //   }

  //   if (drawText) {

  //     window.draw(text);
  //   }

  //   ImGui::SFML::Render(window);
  //   window.display();
  // }

  EnityManager entity_manager;
  entity_manager.loadFromFile("../config/config.txt");

  std::cout << "Number of entities = " << entity_manager.p_id << "\n";

  for (auto circles : entity_manager.m_circlesVec) {
    std::cout << circles->name << "\n";
  }

  for (auto circles : entity_manager.m_rectanglesVec) {
    std::cout << circles->name << "\n";
  }

  return 0;
}