#include "include/game.h"

#include <iostream>

Game::Game(const std::string& config) { init(config); }

void Game::init(const std::string& path) {
  // TODO: read in config here

  m_window.create(sf::VideoMode(1280, 720), "Simple Circle Game");
  m_window.setFramerateLimit(60);

  spawnPlayer();
  spawnEnemy();
}

void Game::run() {
  while (m_running) {
    m_entities.update();
    sEnemySpawner();
    sMovement();
    sCollision();
    sUserInput();
    sRender();
    m_currentFrame++;
  }
}

void Game::setPaused(bool paused) {}

void Game::spawnPlayer() {
  auto entity = m_entities.addEntity("player");

  entity->cTransform = std::make_shared<CTransform>(Vec2(200.0f, 200.0f),
                                                    Vec2(1.0f, 1.0f), 0.0f);

  entity->cShape = std::make_shared<CShape>(32.0f, 8, sf::Color(10, 10, 10),
                                            sf::Color(255, 0, 0), 4.0f);

  entity->cInput = std::make_shared<CInput>();

  m_player = entity;
}

void Game::spawnEnemy() {
  auto entity = m_entities.addEntity("enemy");

  entity->cTransform = std::make_shared<CTransform>(Vec2(200.0f, 200.0f),
                                                    Vec2(1.0f, 0.0f), 0.0f);

  entity->cShape = std::make_shared<CShape>(32.0f, 5, sf::Color(10, 10, 10),
                                            sf::Color(255, 0, 0), 4.0f);

  entity->cInput = std::make_shared<CInput>();

  m_lastEnemySpawnTime = m_currentFrame;
}

void Game::spawnSmallEnemies(std::shared_ptr<Entity> entity) {
  static_cast<void>(entity);
}

void Game::spawnBullet(std::shared_ptr<Entity> entity, const Vec2& target) {
  auto bullet = m_entities.addEntity("bullet");

  bullet->cTransform =
      std::make_shared<CTransform>(target, Vec2(0.0f, 0.0f), 0.0f);

  bullet->cShape = std::make_shared<CShape>(10.0f, 10, sf::Color(255, 255, 10),
                                            sf::Color(255, 255, 255), 4.0f);
}

void Game::spawnSpecialWeapon(std::shared_ptr<Entity> entity) {}

void Game::sMovement() {
  for (auto entitiy : m_entities.getEntities()) {
    entitiy->cTransform->position.x += entitiy->cTransform->velocity.x;
    entitiy->cTransform->position.y += entitiy->cTransform->velocity.y;
  }
}

void Game::sCollision() {}

void Game::sEnemySpawner() {}

void Game::sRender() {
  m_window.clear();
  for (auto entitiy : m_entities.getEntities()) {
    entitiy->cShape->circle.setPosition(entitiy->cTransform->position.x,
                                        entitiy->cTransform->position.y);
    entitiy->cTransform->angle += 1.0f;

    entitiy->cShape->circle.setRotation(entitiy->cTransform->angle);

    m_window.draw(entitiy->cShape->circle);
  }

  m_window.display();
}

void Game::sUserInput() {
  sf::Event event;
  while (m_window.pollEvent(event)) {
    if (event.type == sf::Event::Closed) {
      m_running = false;
    }

    if (event.type == sf::Event::KeyPressed) {
      switch (event.key.code) {
        case sf::Keyboard::W:
          std::cout << "W key Pressed\n";
          /* code */
          break;
        default:
          break;
      }
    }

    if (event.type == sf::Event::KeyReleased) {
      switch (event.key.code) {
        case sf::Keyboard::W:
          std::cout << "W key Released\n";
          /* code */
          break;
        default:
          break;
      }
    }

    if (event.type == sf::Event::MouseButtonPressed) {
      if (event.mouseButton.button == sf::Mouse::Left) {
        std::cout << "Left Mouse Button Clicked at (" << event.mouseButton.x
                  << "," << event.mouseButton.y << ")\n";
        spawnBullet(m_player, Vec2(event.mouseButton.x, event.mouseButton.y));
      }
      if (event.mouseButton.button == sf::Mouse::Right) {
        std::cout << "Right Mouse Button Clicked at (" << event.mouseButton.x
                  << "," << event.mouseButton.y << ")\n";
      }
    }
  }
}