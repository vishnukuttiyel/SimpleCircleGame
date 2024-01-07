#include "include/game.h"

#include <fstream>
#include <iostream>

Game::Game(const std::string& config) { init(config); }

void Game::init(const std::string& path) {
  // TODO: read in config here

  std::ifstream fin(path);
  if (!fin.is_open()) {
    std::cout << "File opening failed\n";
  }
  std::string token;

  while (fin >> token) {
    // std::cout << token;

    if (token == "Window") {
      int width, height, frameRate;
      bool fullScreen;

      fin >> width >> height >> frameRate >> fullScreen;

      std::cout << width << height << frameRate << fullScreen;

      if (fullScreen) {
        m_window.create(sf::VideoMode::getFullscreenModes()[0],
                        "Simple Circle Game", sf::Style::Fullscreen);
      } else {
        m_window.create(sf::VideoMode(width, height), "Simple Circle Game");
      }
      m_window.setFramerateLimit(frameRate);
    }

    if (token == "Font") {
      std::string fontPath;
      int size, r, g, b;
      fin >> fontPath >> size >> r >> g >> b;
      if (!m_font.loadFromFile(fontPath)) {
        // if we can't load the font exit
        std::cerr << "Could not load font\n";
        exit(-1);
      }
      m_text.setFont(m_font);
      m_text.setFillColor(sf::Color(r, g, b));
      m_text.setCharacterSize(size);
      m_text.setPosition(0, 0);
      m_text.setString("Origin");
    }
    if (token == "Player") {
      fin >> m_playerconfig.SR >> m_playerconfig.CR >> m_playerconfig.S >>
          m_playerconfig.FR >> m_playerconfig.FG >> m_playerconfig.FB >>
          m_playerconfig.OR >> m_playerconfig.OG >> m_playerconfig.OB >>
          m_playerconfig.OT >> m_playerconfig.V;
    }
    if (token == "Enemy") {
      fin >> m_enemyconfig.SR >> m_enemyconfig.CR >> m_enemyconfig.SMIN >>
          m_enemyconfig.SMAX >> m_enemyconfig.OR >> m_enemyconfig.OG >>
          m_enemyconfig.OB >> m_enemyconfig.OT >> m_enemyconfig.VMIN >>
          m_enemyconfig.VMAX >> m_enemyconfig.L >> m_enemyconfig.SI;
    }
    if (token == "Bullet") {
      fin >> m_bulletconfig.SR >> m_bulletconfig.CR >> m_bulletconfig.S >>
          m_bulletconfig.FR >> m_bulletconfig.FG >> m_bulletconfig.FB >>
          m_bulletconfig.OR >> m_bulletconfig.OG >> m_bulletconfig.OB >>
          m_bulletconfig.OT >> m_bulletconfig.V >> m_bulletconfig.L;
    }
  }

  m_window.create(sf::VideoMode(1280, 720), "Simple Circle Game");
  spawnPlayer();
}

void Game::run() {
  while (m_running) {
    if (!m_player->isActive()) {
      spawnPlayer();
    }
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

  auto window_center_x = m_window.getSize().x / 2;
  auto window_center_y = m_window.getSize().y / 2;
  entity->cTransform = std::make_shared<CTransform>(
      Vec2(window_center_x, window_center_y), Vec2(m_playerconfig.S, m_playerconfig.S), 0.0f);

  entity->cShape = std::make_shared<CShape>(
      m_playerconfig.SR, m_playerconfig.V,
      sf::Color(m_playerconfig.FR, m_playerconfig.FG, m_playerconfig.FB),
      sf::Color(m_playerconfig.OR, m_playerconfig.OG, m_playerconfig.OB),
      m_playerconfig.OT);

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
  m_player->cTransform->velocity = Vec2{0, 0};

  if (m_player->cInput->up) {
    m_player->cTransform->velocity.y = -5.f;
  }
  if (m_player->cInput->down) {
    m_player->cTransform->velocity.y = 5.f;
  }
  if (m_player->cInput->left) {
    m_player->cTransform->velocity.x = -5.f;
  }
  if (m_player->cInput->right) {
    m_player->cTransform->velocity.x = 5.f;
  }

  for (auto entitiy : m_entities.getEntities()) {
    entitiy->cTransform->position.x += entitiy->cTransform->velocity.x;
    entitiy->cTransform->position.y += entitiy->cTransform->velocity.y;
  }
}

void Game::sCollision() {
  for (auto enemy : m_entities.getEntities("enemy")) {
    {  // check if enemy killed player
      auto distance_square = m_player->cTransform->position.distanceSquare(
          enemy->cTransform->position);
      auto min_collistion_distance = m_player->cShape->circle.getRadius() +
                                     enemy->cShape->circle.getRadius();

      if (distance_square <=
          min_collistion_distance * min_collistion_distance) {
        enemy->destroy();
        m_player->destroy();
      }
    }

    // Check if any bullet killed enemy
    for (auto bullet : m_entities.getEntities("bullet")) {
      auto distance_square = bullet->cTransform->position.distanceSquare(
          enemy->cTransform->position);
      auto min_collistion_distance = bullet->cShape->circle.getRadius() +
                                     enemy->cShape->circle.getRadius();

      if (distance_square <=
          min_collistion_distance * min_collistion_distance) {
        enemy->destroy();
        bullet->destroy();
      }
    }
  }
}

void Game::sEnemySpawner() {
  if (m_currentFrame - m_lastEnemySpawnTime > 150) {
    spawnEnemy();
  }
}

void Game::sRender() {
  m_window.clear();
  for (auto entitiy : m_entities.getEntities()) {
    entitiy->cShape->circle.setPosition(entitiy->cTransform->position.x,
                                        entitiy->cTransform->position.y);
    entitiy->cTransform->angle += 1.0f;

    entitiy->cShape->circle.setRotation(entitiy->cTransform->angle);

    m_window.draw(entitiy->cShape->circle);
  }
  m_window.draw(m_text);

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
          m_player->cInput->up = true;
          break;
        case sf::Keyboard::A:
          std::cout << "A key Pressed\n";
          m_player->cInput->left = true;
          break;
        case sf::Keyboard::S:
          std::cout << "S key Pressed\n";
          m_player->cInput->down = true;
          break;
        case sf::Keyboard::D:
          std::cout << "D key Pressed\n";
          m_player->cInput->right = true;
          break;
        default:
          break;
      }
    }

    if (event.type == sf::Event::KeyReleased) {
      switch (event.key.code) {
        case sf::Keyboard::W:
          std::cout << "W key Released\n";
          m_player->cInput->up = false;
          break;
        case sf::Keyboard::A:
          std::cout << "A key Released\n";
          m_player->cInput->left = false;
          break;
        case sf::Keyboard::S:
          std::cout << "S key Released\n";
          m_player->cInput->down = false;
          break;
        case sf::Keyboard::D:
          std::cout << "D key Released\n";
          m_player->cInput->right = false;
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