#include <fstream>
#include <iostream>
#include <memory>

#include "imgui-SFML.h"
#include "imgui.h"
#include <SFML/Graphics.hpp>

int
main(int argc, char* argv[])
{
  // create a wundow of size w*h pixels
  // top-left of the window is (0,0) and bottom-right is (w,h)
  // you will have to read these from the config file

  const int wWidth = 1280;
  const int wHeight = 720;

  sf::RenderWindow window(sf::VideoMode(wWidth, wHeight), "SFML Works!");
  window.setFramerateLimit(60);

  // initialize IMGUI and create a clock used for its timing
  ImGui::SFML::Init(window);
  sf::Clock deltaClock;

  // scale the imgui ui by a give factor, does not affect text size
  ImGui::GetStyle().ScaleAllSizes(1.0f);

  // the imgui color {r, g, b} wheel requires floats from 0-1
  float c[3] = { 0.0f, 1.0f, 1.0f };

  // shape
  float circleRadius = 50;
  int circleSegments = 32; // number of segments to draw the circle
  float circleVelocityX = 1.0f;
  float circleVelocityY = 0.5f;
  bool drawCircle = true;
  bool drawText = true;

  // create the sfml circle shape based on our paramteres
  sf::CircleShape circle(circleRadius, circleSegments);

  sf::Font myFont;

  // attempt to load the font from a file
  if (!myFont.loadFromFile("fonts/tech.ttf")) {
    // if we can't load the font exit
    std::cerr << "Could not load font\n";
    exit(-1);
  }

  sf::Text text("Sample Text", myFont, 24);

  text.setPosition(0, wHeight - (float)text.getCharacterSize());

  // set up a charcter array to set the text
  char displayString[255] = "Sample Text";

  // main loop - continues for each frame, while window is open
  while (window.isOpen()) {

    sf::Event event;
    while (window.pollEvent(event)) {
      ImGui::SFML::ProcessEvent(window, event);

      // This event is treggered when window is closed
      if (event.type == sf::Event::Closed) {
        window.close();
      }

      // this event is triggered when any key is pressed
      if (event.type == sf::Event::KeyPressed) {

        // print the key that was pressed to the console
        std::cout << " Key pressed with code =" << event.key.code << "\n";

        if (event.key.code == sf::Keyboard::X) {

          // reverse the x direction of the circle on the screen
          circleVelocityX *= -1.0f;
        }
      }
    }

    // update imgui of this frame with the time that the last frame took
    ImGui::SFML::Update(window, deltaClock.restart());

    ImGui::Begin("Window title");
    ImGui::Text("Window text!");
    ImGui::Checkbox("Draw Circle", &drawCircle);
    ImGui::SameLine();
    ImGui::Checkbox("Draw Text", &drawText);
    ImGui::SameLine();
    ImGui::SliderFloat("Radius", &circleRadius, 0.0f, 300.0f);
    ImGui::SliderInt("Sides", &circleSegments, 3, 64);
    ImGui::ColorEdit3("Color Circle", c);
    ImGui::InputText("Text", displayString, 255);

    if (ImGui::Button("Set Text")) {
      text.setString(displayString);
    }
    ImGui::SameLine();

    if (ImGui::Button("Reset Circle")) {
      circle.setPosition(0, 0);
    }
    ImGui::End();

    // set the circle properies because they may have been updated with the ui
    circle.setFillColor(sf::Color(c[0] * 255, c[1] * 255, c[2] * 255));
    circle.setPointCount(circleSegments);
    circle.setRadius(circleRadius);

    // basic animation - move in each frame
    circle.setPosition(circle.getPosition().x + circleVelocityX,
                       circle.getPosition().y + circleVelocityY);

    // basic rendering function calls
    window.clear();
    if (drawCircle) {

      window.draw(circle);
    }

    if (drawText) {

      window.draw(text);
    }

    ImGui::SFML::Render(window);
    window.display();
  }

  return 0;
}