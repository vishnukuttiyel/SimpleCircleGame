#pragma once

#include <include/enitiymanager.h>

#include <SFML/Graphics.hpp>

struct PlayerCongig {
    int SR, CR, FR, FG, FB, OR, OG, OB, OT, V;
    float S;
};
struct EnemyCongig {
    int SR, CR, FR, FG, FB, OR, OG, OB, OT, VMIN, VMAX, L, SI;
    float SMIN, SMAX;
};
struct BulletCongig {
    int SR, CR, FR, FG, FB, OR, OG, OB, OT, V, L;
    float S;
};

class Game {
    sf::RenderWindow m_window;
    EnitityManager m_entities;
    sf::Font m_font;
    sf::Text m_text;
    PlayerCongig m_playerconfig;
    EnemyCongig m_enemyconfig;
    BulletCongig m_bulletconfig;
    int m_score = 0;
    int m_currentFrame = 0;
    int m_lastEnemySpawnTime = 0;
    int m_lastSuperPowerTime = 0;
    bool m_paused = false;
    bool m_running = true;

    std::shared_ptr<Entity> m_player;

    void init(const std::string& config);  // Initialise the gameState with config
    void setPaused(bool m_paused);

    // Systems
    void sCollision();
    void sEnemySpawner();
    void sLifeSpan();
    void sMovement();
    void sRender();
    void sUserInput();

    void spawnBullet(std::shared_ptr<Entity> entity, const Vec2& mousePos);
    void spawnEnemy();
    void spawnPlayer();
    void spawnSmallEnemies(std::shared_ptr<Entity> entity);
    void spawnSpecialWeapon(std::shared_ptr<Entity> entity);

   public:
    Game(const std::string& config);

    void run();
};