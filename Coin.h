#pragma once

#include <SFML/Graphics.hpp>  // Grafika SFML
#include "GameObject.h"        // Klasa bazowa (dziedziczenie)
#include "Constants.h"         // Stałe gry

// ============================================================
//  Coin.h – klasa monety (bonus do zebrania)
//  DZIEDZICZY z GameObject – implementuje draw() i getPosition()
//  POLIMORFIZM – draw() i update() nadpisują metody wirtualne
// ============================================================

class Coin : public GameObject {  // Dziedziczenie publiczne z GameObject
public:
    // Konstruktor – tworzy monetę w podanej pozycji
    explicit Coin(sf::Vector2f pos);

    // Nadpisanie (override) metody wirtualnej – rysuje monetę
    // POLIMORFIZM – inny wygląd niż Obstacle mimo tej samej klasy bazowej
    void draw(sf::RenderWindow& window) const override;

    // Nadpisanie metody wirtualnej – animacja pulsowania monety
    void update(float dt) override;

    // Nadpisanie metody wirtualnej – zwraca pozycję monety
    sf::Vector2f getPosition() const override;

    // Próbuje zebrać monetę – zwraca true jeśli auto jest blisko
    bool tryCollect(sf::Vector2f carPos);

    // Sprawdza czy moneta została już zebrana
    bool isCollected() const;

private:
    sf::CircleShape m_shape;     // Kółko złotej monety
    bool            m_collected; // Czy zebrana przez gracza
    float           m_animTimer; // Timer animacji pulsowania
    float           m_scale;     // Aktualny rozmiar (pulsowanie)
};
