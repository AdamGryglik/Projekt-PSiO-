#pragma once

#include <SFML/Graphics.hpp>  // Grafika SFML

// ============================================================
//  EndScreen.h – ekran podsumowania wyścigu
//  Wyświetla czas, zebrania monet, czy pobito rekord
//  Przyciski: "MENU GŁÓWNE" i "RANKING"
// ============================================================

// Enum stanu wyboru na ekranie końcowym
enum class EndChoice {
    None,    // Gracz jeszcze nic nie wybrał
    Menu,    // Wróć do menu głównego
    Ranking  // Pokaż ekran rankingu
};

class EndScreen {
public:
    // Konstruktor
    EndScreen();

    // Ustaw wyniki do wyświetlenia
    void setResults(float timeSec, int coins, bool isRecord);

    // Obsługuje zdarzenia – zwraca wybór gracza
    EndChoice handleEvent(const sf::Event& event);

    // Rysuje ekran końcowy
    void draw(sf::RenderWindow& window) const;

private:
    sf::Font m_font;   // Czcionka
    bool     m_loaded; // Czy font załadowany

    float m_time;    // Czas wyścigu w sekundach
    int   m_coins;   // Zebrane monety
    bool  m_record;  // Czy pobito rekord

    sf::RectangleShape m_bg;        // Tło
    sf::RectangleShape m_menuBtn;   // Przycisk "MENU GŁÓWNE"
    sf::RectangleShape m_rankBtn;   // Przycisk "RANKING"

    sf::Text m_menuBtnText; // Tekst przycisku menu
    sf::Text m_rankBtnText; // Tekst przycisku rankingu

    // Pomocnicza tworzenia tekstu
    sf::Text makeText(const std::string& str, unsigned size,
                      sf::Color color, sf::Vector2f pos) const;
};
