#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>

#include "GameObject.h"      // Klasa bazowa (polimorfizm)
#include "Car.h"
#include "Track.h"
#include "Obstacle.h"
#include "Coin.h"
#include "HUD.h"
#include "Menu.h"
#include "RankingScreen.h"
#include "EndScreen.h"
#include "Ranking.h"
#include "Constants.h"

// Stan maszyny stanów gry
enum class GameState {
    Menu,    // Ekran menu głównego
    Playing, // Aktywny wyścig
    End,     // Ekran podsumowania
    Ranking  // Ekran rankingu
};

// ============================================================
//  Game.h – główna klasa zarządzająca całą grą
//  Używa POLIMORFIZMU przez vector<unique_ptr<GameObject>>
//  Wszystkie obiekty gry przechowywane w jednym kontenerze
// ============================================================

class Game {
public:
    Game();
    void run(); // Główna pętla gry

private:
    sf::RenderWindow m_window;  // Okno gry SFML
    GameState        m_state;   // Aktualny stan maszyny stanów

    // Auto gracza – wskaźnik na klasę pochodną przez klasę bazową
    std::unique_ptr<Car>   m_car;    // unique_ptr – RAII, brak wycieków pamięci
    std::unique_ptr<Track> m_track;  // Aktualny tor

    // ============================================================
    //  POLIMORFIZM + KONTENER:
    //  Wszystkie obiekty gry (monety i przeszkody) przechowywane
    //  w jednym kontenerze przez wskaźnik na klasę bazową.
    //  Wywołanie draw()/update() uruchamia właściwą implementację
    //  każdego obiektu (Coin::draw lub Obstacle::draw)
    // ============================================================
    std::vector<std::unique_ptr<GameObject>> m_gameObjects; // Kontener polimorficzny

    // Interfejs użytkownika
    HUD           m_hud;
    Menu          m_menu;
    RankingScreen m_rankingScreen;
    EndScreen     m_endScreen;
    Ranking       m_ranking;

    // Stan wyścigu
    float m_raceTime;
    int   m_lapCount;
    int   m_coins_collected;
    bool  m_lapStarted;
    std::vector<bool> m_checkpointPassed;

    // Metody prywatne
    void startRace(int carId, int trackId);
    void spawnObstaclesAndCoins();
    void processEvents();
    void update(float dt);
    void draw();
    void checkLap();
};
