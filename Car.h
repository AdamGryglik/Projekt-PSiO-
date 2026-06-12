#pragma once

#include <SFML/Graphics.hpp>  // Grafika SFML
#include "GameObject.h"        // Klasa bazowa (dziedziczenie)
#include "Constants.h"         // Stałe gry

// ============================================================
//  Car.h – klasa auta gracza
//  DZIEDZICZY z GameObject – implementuje draw() i getPosition()
//  POLIMORFIZM – draw() i update() nadpisują metody wirtualne
// ============================================================

class Car : public GameObject {  // Dziedziczenie publiczne z GameObject
public:
    // Konstruktor – tworzy auto w pozycji startowej o podanym kolorze
    Car(sf::Vector2f startPos, sf::Color color);

    // Nadpisanie (override) – aktualizuje fizykę auta z bezwładnością
    void update(float dt) override;

    // Specjalna wersja update dla auta z wejściem klawiatury
    void update(float dt, bool gasDown, bool brakeDown,
                bool leftDown, bool rightDown);

    // Nadpisanie (override) – rysuje auto z lusterkami
    void draw(sf::RenderWindow& window) const override;

    // Nadpisanie (override) – zwraca pozycję środka auta
    sf::Vector2f getPosition() const override;

    // Resetuje auto do pozycji startowej
    void reset(sf::Vector2f startPos);

    // Zwraca aktualny kąt obrotu auta w stopniach
    float getAngle() const;

    // Zwraca aktualną prędkość skalarną (px/s)
    float getSpeed() const;

    // Zwraca obrys kolizyjny AABB
    sf::FloatRect getBounds() const;

    // Wywołuje efekt odbicia przy kolizji
    void applyCollision();

    // Ustawia pozycję auta
    void setPosition(sf::Vector2f pos);

private:
    sf::RectangleShape m_shape;    // Prostokąt nadwozia
    float              m_angle;    // Kąt obrotu w stopniach
    float              m_speed;    // Prędkość wzdłuż osi auta [px/s]
    sf::Color          m_color;    // Kolor nadwozia
    sf::Vector2f       m_startPos; // Zapamiętana pozycja startowa
};
