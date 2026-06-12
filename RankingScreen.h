#pragma once

#include <SFML/Graphics.hpp>  // Grafika SFML
#include "Ranking.h"           // Dane rankingu

// ============================================================
//  RankingScreen.h – ekran wyświetlający top 3 wyników
//  Pokazuje ranking czasów i monet dla każdego toru
// ============================================================

class RankingScreen {
public:
    // Konstruktor – przygotowuje elementy UI
    RankingScreen();

    // Obsługuje zdarzenia (ESC/klik = wróć do menu)
    // Zwraca true gdy gracz chce wrócić do menu
    bool handleEvent(const sf::Event& event);

    // Rysuje ekran rankingu
    void draw(sf::RenderWindow& window, const Ranking& ranking) const;

private:
    sf::Font m_font;     // Czcionka UI
    bool     m_loaded;   // Czy font załadowany

    sf::RectangleShape m_bg;  // Tło ekranu

    // Pomocnicza – tworzy tekst z daną czcionką i ustawieniami
    sf::Text makeText(const std::string& str, unsigned size,
                      sf::Color color, sf::Vector2f pos) const;
};
