#pragma once

#include <SFML/Graphics.hpp>  // Grafika SFML
#include "GameObject.h"        // Klasa bazowa (dziedziczenie)
#include "Constants.h"         // Stałe gry

// ============================================================
//  Obstacle.h – klasa przeszkody (beczka/pachołek)
//  DZIEDZICZY z GameObject – implementuje draw() i getPosition()
//  POLIMORFIZM – draw() nadpisuje metodę wirtualną z klasy bazowej
// ============================================================

class Obstacle : public GameObject {  // Dziedziczenie publiczne z GameObject
public:
    // Konstruktor – tworzy przeszkodę w podanej pozycji
    // type: 0 = beczka (brązowa), 1 = pachołek (pomarańczowy)
    Obstacle(sf::Vector2f pos, int type = 0);

    // Nadpisanie (override) metody wirtualnej z klasy bazowej
    // POLIMORFIZM – rysuje przeszkodę odpowiednio do jej typu
    void draw(sf::RenderWindow& window) const override;

    // Nadpisanie metody wirtualnej – aktualizacja (animacja obrotu)
    void update(float dt) override;

    // Nadpisanie metody wirtualnej – zwraca pozycję przeszkody
    sf::Vector2f getPosition() const override;

    // Sprawdza kolizję auta z przeszkodą
    bool checkCollision(sf::Vector2f carPos) const;

private:
    sf::CircleShape m_shape;   // Kółkowy kształt przeszkody
    int             m_type;    // Typ: 0=beczka, 1=pachołek
    float           m_rotation; // Kąt obrotu dla animacji
};
