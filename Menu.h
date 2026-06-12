#pragma once

#include <SFML/Graphics.hpp>  // Grafika SFML

// ============================================================
//  Menu.h – ekran menu głównego
//  Pozwala wybrać auto (czerwone/niebieskie) i tor (trawa/piasek/miasto)
// ============================================================

class Menu {
public:
    // Konstruktor – inicjalizuje elementy UI menu
    Menu();

    // Obsługuje zdarzenia myszy/klawiatury w menu
    // Zwraca true gdy gracz zakończył wybór i chce zacząć grę
    bool handleEvent(const sf::Event& event, sf::RenderWindow& window);

    // Rysuje ekran menu
    void draw(sf::RenderWindow& window) const;

    // Zwraca wybrany typ auta: 0 = czerwone, 1 = niebieskie
    int getSelectedCar() const;

    // Zwraca wybrany tor: 0 = trawa, 1 = piasek, 2 = miasto
    int getSelectedTrack() const;

    // Resetuje menu do stanu początkowego
    void reset();

private:
    sf::Font m_font;          // Czcionka UI
    bool     m_loaded;        // Czy font załadowany

    // Wybory gracza
    int m_selectedCar;        // Aktualnie wybrany indeks auta
    int m_selectedTrack;      // Aktualnie wybrany indeks toru

    // Kształty ramek wyboru aut
    sf::RectangleShape m_carBox[2];  // Dwa prostokąty wyboru auta
    sf::RectangleShape m_carShape[2]; // Miniaturki aut

    // Tekst nagłówka "WYBIERZ AUTO"
    sf::Text m_titleCar;

    // Tekst nagłówka "WYBIERZ TOR"
    sf::Text m_titleTrack;

    // Przyciski torów
    sf::RectangleShape m_trackBtn[3]; // 3 przyciski torów
    sf::Text           m_trackLabel[3]; // Etykiety torów

    // Przycisk START
    sf::RectangleShape m_startBtn;
    sf::Text           m_startText;

    // Tło menu
    sf::RectangleShape m_bg;

    // Pomocnicza – ładuje font
    bool loadFont();

    // Pomocnicza – inicjalizuje elementy UI
    void setupUI();

    // Kolory zaznaczonego i niezaznaczonego przycisku
    static const sf::Color SEL_COLOR;
    static const sf::Color UNSEL_COLOR;
};
