#pragma once

#include <SFML/Graphics.hpp>  // Grafika SFML
#include <string>              // std::string

// ============================================================
//  HUD.h – wyświetlanie informacji na ekranie podczas gry
//  Pokazuje: czas, prędkość, numer okrążenia, monety
// ============================================================

class HUD {
public:
    // Konstruktor – ładuje font i przygotuje teksty
    HUD();

    // Aktualizuje wszystkie wartości wyświetlane na HUD
    void update(float elapsedSeconds, float speedKmh, int lap, int totalLaps, int coins);

    // Rysuje HUD na ekranie (na wierzchu wszystkiego)
    void draw(sf::RenderWindow& window) const;

    // Zwraca czy font został poprawnie wczytany
    bool isLoaded() const;

private:
    sf::Font m_font;           // Czcionka do wyświetlania tekstu

    sf::Text m_timeText;       // "CZAS: 01:23"
    sf::Text m_speedText;      // "V: 120 km/h"
    sf::Text m_lapText;        // "OKR: 2/3"
    sf::Text m_coinText;       // "PKT: 150"

    sf::RectangleShape m_bar;  // Ciemne tło pod tekstami HUD

    bool m_loaded;             // Czy font załadowany poprawnie

    // Formatuje sekundy na string "MM:SS"
    static std::string formatTime(float seconds);
};
