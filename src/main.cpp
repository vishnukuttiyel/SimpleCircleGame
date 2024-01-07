#include <SFML/Graphics.hpp>
#include <memory>

#include "include/game.h"

int main(int argc, char* argv[]) {
  Game g("../config/config.txt");
  g.run();
}
