#include "Game.h"
#include <cmath>
#include <iostream>

// ============================================================
//  Konstruktor
// ============================================================
Game::Game()
    : m_window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Jezdzcy Pro+",
               sf::Style::Titlebar | sf::Style::Close)
    , m_state(GameState::Menu)
    , m_raceTime(0.f)
    , m_lapCount(0)
    , m_coins_collected(0)
    , m_lapStarted(false)
{
    m_window.setFramerateLimit(FRAMERATE_LIMIT);
}

// ============================================================
//  run – główna pętla gry
// ============================================================
void Game::run()
{
    sf::Clock clock;
    while (m_window.isOpen()) {
        float dt = clock.restart().asSeconds();
        if (dt > 0.05f) dt = 0.05f; // Ogranicz dt przy lagach

        processEvents();
        if (m_state == GameState::Playing) {
            update(dt);
        }
        draw();
    }
}

// ============================================================
//  processEvents – obsługa zdarzeń SFML
// ============================================================
void Game::processEvents()
{
    sf::Event event;
    while (m_window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            m_window.close();
        }

        if (event.type == sf::Event::KeyPressed &&
            event.key.code == sf::Keyboard::Escape) {
            if (m_state == GameState::Playing) {
                m_state = GameState::Menu;
                m_menu.reset();
            } else if (m_state == GameState::Ranking) {
                m_state = GameState::Menu;
            } else {
                m_window.close();
            }
        }

        switch (m_state) {
            case GameState::Menu:
                if (m_menu.handleEvent(event, m_window)) {
                    startRace(m_menu.getSelectedCar(),
                              m_menu.getSelectedTrack());
                }
                break;
            case GameState::Playing:
                break;
            case GameState::End: {
                EndChoice choice = m_endScreen.handleEvent(event);
                if (choice == EndChoice::Menu) {
                    m_state = GameState::Menu;
                    m_menu.reset();
                } else if (choice == EndChoice::Ranking) {
                    m_state = GameState::Ranking;
                }
                break;
            }
            case GameState::Ranking:
                if (m_rankingScreen.handleEvent(event)) {
                    m_state = GameState::Menu;
                    m_menu.reset();
                }
                break;
        }
    }
}

// ============================================================
//  startRace – inicjalizuje nowy wyścig
// ============================================================
void Game::startRace(int carId, int trackId)
{
    m_track = std::make_unique<Track>(trackId);

    sf::Color carColor = (carId == 0)
        ? sf::Color(CAR1_R, CAR1_G, CAR1_B)
        : sf::Color(CAR2_R, CAR2_G, CAR2_B);

    m_car = std::make_unique<Car>(m_track->getStartPosition(), carColor);

    m_raceTime        = 0.f;
    m_lapCount        = 0;
    m_coins_collected = 0;
    m_lapStarted      = false;

    m_checkpointPassed.assign(m_track->getCheckpoints().size(), false);

    spawnObstaclesAndCoins();
    m_state = GameState::Playing;
}

// ============================================================
//  spawnObstaclesAndCoins – rozmieszcza obiekty na torze
//  POLIMORFIZM: Coin i Obstacle dodawane do wspólnego kontenera
//  przez wskaźnik na klasę bazową GameObject
// ============================================================
void Game::spawnObstaclesAndCoins()
{
    // Wyczyść kontener polimorficzny
    m_gameObjects.clear();

    if (!m_track) return;

    float W = (float)WINDOW_WIDTH;
    float H = (float)WINDOW_HEIGHT;

    // --- Dodaj przeszkody do kontenera polimorficznego ---
    // make_unique<Obstacle> tworzy obiekt pochodny,
    // przechowywany przez wskaźnik na klasę bazową GameObject
    struct ObstPos { float rx, ry; int type; };
    ObstPos obstPositions[] = {
        {0.25f, 0.20f, 0}, {0.75f, 0.20f, 1},
        {0.85f, 0.50f, 0}, {0.75f, 0.80f, 1},
        {0.25f, 0.80f, 0}, {0.15f, 0.50f, 1},
    };

    for (auto& op : obstPositions) {
        sf::Vector2f pos(W * op.rx, H * op.ry);
        if (m_track->isOnTrack(pos)) {
            // POLIMORFIZM: dodajemy Obstacle jako GameObject
            m_gameObjects.push_back(
                std::make_unique<Obstacle>(pos, op.type));
        }
    }

    // --- Dodaj monety do kontenera polimorficznego ---
    struct CoinPos { float rx, ry; };
    CoinPos coinPositions[] = {
        {0.30f, 0.18f}, {0.50f, 0.15f}, {0.70f, 0.18f},
        {0.82f, 0.35f}, {0.82f, 0.65f},
        {0.70f, 0.82f}, {0.50f, 0.85f}, {0.30f, 0.82f},
        {0.18f, 0.65f}, {0.18f, 0.35f},
    };

    for (auto& cp : coinPositions) {
        sf::Vector2f pos(W * cp.rx, H * cp.ry);
        if (m_track->isOnTrack(pos)) {
            // POLIMORFIZM: dodajemy Coin jako GameObject
            m_gameObjects.push_back(
                std::make_unique<Coin>(pos));
        }
    }
}

// ============================================================
//  update – główna logika gry
// ============================================================
void Game::update(float dt)
{
    if (!m_car || !m_track) return;

    // --- Wejście klawiatury ---
    bool gas   = sf::Keyboard::isKeyPressed(sf::Keyboard::W);
    bool brake = sf::Keyboard::isKeyPressed(sf::Keyboard::S);
    bool left  = sf::Keyboard::isKeyPressed(sf::Keyboard::A);
    bool right = sf::Keyboard::isKeyPressed(sf::Keyboard::D);

    // --- Aktualizuj auto ---
    m_car->update(dt, gas, brake, left, right);

    // --- POLIMORFIZM: aktualizuj wszystkie obiekty gry ---
    // Wywołanie update() przez wskaźnik bazowy uruchamia
    // właściwą implementację (Coin::update lub Obstacle::update)
    for (auto& obj : m_gameObjects) {
        obj->update(dt);
    }

    // --- Czas wyścigu ---
    if (m_lapStarted) m_raceTime += dt;

    // --- Kolizje z przeszkodami ---
    sf::Vector2f carPos = m_car->getPosition();
    for (auto& obj : m_gameObjects) {
        // dynamic_cast – sprawdza czy obiekt jest przeszkodą
        // DYNAMIC CASTING przez wskaźnik klasy bazowej
        Obstacle* obs = dynamic_cast<Obstacle*>(obj.get());
        if (obs && obs->isActive() && obs->checkCollision(carPos)) {
            m_car->applyCollision();
            break;
        }
    }

    // --- Blokada wyjazdu poza tor ---
    carPos = m_car->getPosition();
    sf::Vector2f clampedPos = carPos;
    if (m_track->pushBackOnTrack(clampedPos)) {
        m_car->setPosition(clampedPos);
        m_car->applyCollision();
    }

    // --- Zbieranie monet ---
    carPos = m_car->getPosition();
    for (auto& obj : m_gameObjects) {
        // dynamic_cast – sprawdza czy obiekt jest monetą
        Coin* coin = dynamic_cast<Coin*>(obj.get());
        if (coin && coin->tryCollect(carPos)) {
            m_coins_collected += COIN_POINTS;
        }
    }

    // --- Okrążenia ---
    checkLap();

    // --- HUD ---
    float speedKmh = std::fabs(m_car->getSpeed()) * 0.36f;
    m_hud.update(m_raceTime, speedKmh,
                 std::max(1, m_lapCount),
                 TOTAL_LAPS,
                 m_coins_collected);
}

// ============================================================
//  checkLap – detekcja okrążeń
// ============================================================
void Game::checkLap()
{
    if (!m_car || !m_track) return;

    sf::FloatRect carBounds = m_car->getBounds();

    const auto& checkpoints = m_track->getCheckpoints();
    for (int i = 0; i < (int)checkpoints.size(); i++) {
        if (!m_checkpointPassed[i] && checkpoints[i].intersects(carBounds)) {
            m_checkpointPassed[i] = true;
        }
    }

    sf::FloatRect startLine = m_track->getStartLine();
    if (startLine.intersects(carBounds)) {
        if (!m_lapStarted) {
            m_lapStarted = true;
            m_lapCount   = 1;
        } else {
            bool allPassed = true;
            for (bool b : m_checkpointPassed) {
                if (!b) { allPassed = false; break; }
            }

            if (allPassed) {
                if (m_lapCount >= TOTAL_LAPS) {
                    bool isRecord = m_ranking.addResult(
                        m_track->getId(), m_raceTime, m_coins_collected);
                    m_endScreen.setResults(m_raceTime, m_coins_collected, isRecord);
                    m_state = GameState::End;
                } else {
                    m_lapCount++;
                    std::fill(m_checkpointPassed.begin(),
                              m_checkpointPassed.end(), false);
                }
            }
        }
    }
}

// ============================================================
//  draw – rysuje aktualny stan gry
// ============================================================
void Game::draw()
{
    m_window.clear();

    switch (m_state) {
        case GameState::Menu:
            m_menu.draw(m_window);
            break;

        case GameState::Playing:
            if (m_track) m_track->draw(m_window);

            // POLIMORFIZM: rysuj wszystkie obiekty przez wskaźnik bazowy
            // Wywołanie draw() uruchamia właściwą implementację każdego obiektu
            for (const auto& obj : m_gameObjects) {
                if (obj->isActive()) {
                    obj->draw(m_window); // Coin::draw lub Obstacle::draw
                }
            }

            if (m_car) m_car->draw(m_window);
            m_hud.draw(m_window);
            break;

        case GameState::End:
            m_endScreen.draw(m_window);
            break;

        case GameState::Ranking:
            m_rankingScreen.draw(m_window, m_ranking);
            break;
    }

    m_window.display();
}
