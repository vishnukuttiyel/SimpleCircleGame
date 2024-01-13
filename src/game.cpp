#include "include/game.h"

#include <fstream>
#include <iostream>
#include <random>

int generateRandomNumber(int min, int max) {
    // Generate and return a random number within the specified range
    return (rand() % (max - min + 1)) + min;
}

template <typename T>
T clamp(T value, T min, T max) {
    if (value < min) {
        return min;
    } else if (value > max) {
        return max;
    } else {
        return value;
    }
}

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
                m_window.create(sf::VideoMode::getFullscreenModes()[0], "Simple Circle Game",
                                sf::Style::Fullscreen);
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
                m_playerconfig.FR >> m_playerconfig.FG >> m_playerconfig.FB >> m_playerconfig.OR >>
                m_playerconfig.OG >> m_playerconfig.OB >> m_playerconfig.OT >> m_playerconfig.V;
        }
        if (token == "Enemy") {
            fin >> m_enemyconfig.SR >> m_enemyconfig.CR >> m_enemyconfig.SMIN >>
                m_enemyconfig.SMAX >> m_enemyconfig.OR >> m_enemyconfig.OG >> m_enemyconfig.OB >>
                m_enemyconfig.OT >> m_enemyconfig.VMIN >> m_enemyconfig.VMAX >> m_enemyconfig.L >>
                m_enemyconfig.SI;
        }
        if (token == "Bullet") {
            fin >> m_bulletconfig.SR >> m_bulletconfig.CR >> m_bulletconfig.S >>
                m_bulletconfig.FR >> m_bulletconfig.FG >> m_bulletconfig.FB >> m_bulletconfig.OR >>
                m_bulletconfig.OG >> m_bulletconfig.OB >> m_bulletconfig.OT >> m_bulletconfig.V >>
                m_bulletconfig.L;
        }
    }

    m_window.create(sf::VideoMode(1280, 720), "Simple Circle Game");
    spawnPlayer();
    spawnEnemy();
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
        sf::Color(m_playerconfig.OR, m_playerconfig.OG, m_playerconfig.OB), m_playerconfig.OT);

    entity->cInput = std::make_shared<CInput>();

    m_player = entity;
}

void Game::spawnEnemy() {
    auto entity = m_entities.addEntity("enemy");
    int window_width = m_window.getSize().x;
    int window_height = m_window.getSize().y;

    auto ex =
        static_cast<float>(generateRandomNumber(m_enemyconfig.SR, window_width - m_enemyconfig.SR));
    auto ey = static_cast<float>(
        generateRandomNumber(m_enemyconfig.SR, window_height - m_enemyconfig.SR));

    int number_of_vertices = generateRandomNumber(m_enemyconfig.VMIN, m_enemyconfig.VMAX);
    float sx = static_cast<float>(generateRandomNumber(m_enemyconfig.SMIN, m_enemyconfig.SMAX));
    float sy = static_cast<float>(generateRandomNumber(m_enemyconfig.SMIN, m_enemyconfig.SMAX));

    entity->cTransform = std::make_shared<CTransform>(Vec2(ex, ey), Vec2(sx, sy), 0.0f);

    int FR = generateRandomNumber(0, 255);
    int FG = generateRandomNumber(0, 255);
    int FB = generateRandomNumber(0, 255);

    entity->cShape = std::make_shared<CShape>(
        m_enemyconfig.SR, number_of_vertices, sf::Color(FR, FG, FB),
        sf::Color(m_enemyconfig.OR, m_enemyconfig.OG, m_enemyconfig.OB), m_enemyconfig.OT);

    entity->cInput = std::make_shared<CInput>();

    m_lastEnemySpawnTime = m_currentFrame;
}

void Game::spawnSmallEnemies(std::shared_ptr<Entity> entity) { static_cast<void>(entity); }

void Game::spawnBullet(std::shared_ptr<Entity> entity, const Vec2& target) {
    auto bullet = m_entities.addEntity("bullet");

    bullet->cTransform = std::make_shared<CTransform>(target, Vec2(0.0f, 0.0f), 0.0f);

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

    auto window_width = m_window.getSize().x;
    auto window_height = m_window.getSize().y;

    int player_boundary_x_min = m_playerconfig.SR;
    int player_boundary_x_max = window_width - m_playerconfig.SR;
    int player_boundary_y_min = m_playerconfig.SR;
    int player_boundary_y_max = window_height - m_playerconfig.SR;

    // move player
    m_player->cTransform->position.x += m_player->cTransform->velocity.x;
    m_player->cTransform->position.y += m_player->cTransform->velocity.y;
    m_player->cTransform->position.x = clamp(static_cast<int>(m_player->cTransform->position.x),
                                             player_boundary_x_min, player_boundary_x_max);
    m_player->cTransform->position.y = clamp(static_cast<int>(m_player->cTransform->position.y),
                                             player_boundary_y_min, player_boundary_y_max);

    // move enemies
    for (auto enemy : m_entities.getEntities("enemy")) {
        enemy->cTransform->position.x += enemy->cTransform->velocity.x;
        enemy->cTransform->position.y += enemy->cTransform->velocity.y;
        int enemy_boundary_x_min = m_enemyconfig.SR;
        int enemy_boundary_x_max = window_width - m_enemyconfig.SR;
        int enemy_boundary_y_min = m_enemyconfig.SR;
        int enemy_boundary_y_max = window_height - m_enemyconfig.SR;

        if ((enemy->cTransform->position.x <= enemy_boundary_x_min) ||
            (enemy->cTransform->position.x >= enemy_boundary_x_max)) {
            enemy->cTransform->velocity.x = -enemy->cTransform->velocity.x;
        }

        if ((enemy->cTransform->position.y <= enemy_boundary_y_min) ||
            (enemy->cTransform->position.y >= enemy_boundary_y_max)) {
            enemy->cTransform->velocity.y = -enemy->cTransform->velocity.y;
        }

        enemy->cTransform->position.x = clamp(static_cast<int>(enemy->cTransform->position.x),
                                              enemy_boundary_x_min, enemy_boundary_x_max);
        enemy->cTransform->position.y = clamp(static_cast<int>(enemy->cTransform->position.y),
                                              enemy_boundary_y_min, enemy_boundary_y_max);
    }

    // move bullets
    for (auto entitiy : m_entities.getEntities("bullet")) {
        entitiy->cTransform->position.x += entitiy->cTransform->velocity.x;
        entitiy->cTransform->position.y += entitiy->cTransform->velocity.y;
    }
}

void Game::sCollision() {
    for (auto enemy : m_entities.getEntities("enemy")) {
        {  // check if enemy killed player
            auto distance_square =
                m_player->cTransform->position.distanceSquare(enemy->cTransform->position);
            auto min_collistion_distance =
                m_player->cShape->circle.getRadius() + enemy->cShape->circle.getRadius();

            if (distance_square <= min_collistion_distance * min_collistion_distance) {
                enemy->destroy();
                m_player->destroy();
            }
        }

        // Check if any bullet killed enemy
        for (auto bullet : m_entities.getEntities("bullet")) {
            auto distance_square =
                bullet->cTransform->position.distanceSquare(enemy->cTransform->position);
            auto min_collistion_distance =
                bullet->cShape->circle.getRadius() + enemy->cShape->circle.getRadius();

            if (distance_square <= min_collistion_distance * min_collistion_distance) {
                enemy->destroy();
                bullet->destroy();
            }
        }
    }
}

void Game::sEnemySpawner() {
    if (m_currentFrame - m_lastEnemySpawnTime > m_enemyconfig.SI) {
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
                std::cout << "Left Mouse Button Clicked at (" << event.mouseButton.x << ","
                          << event.mouseButton.y << ")\n";
                spawnBullet(m_player, Vec2(event.mouseButton.x, event.mouseButton.y));
            }
            if (event.mouseButton.button == sf::Mouse::Right) {
                std::cout << "Right Mouse Button Clicked at (" << event.mouseButton.x << ","
                          << event.mouseButton.y << ")\n";
            }
        }
    }
}