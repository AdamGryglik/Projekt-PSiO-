#include "Obstacle.h"
#include <cmath>  // sqrt

// ============================================================
//  Obstacle.cpp – implementacja przeszkody
//  POLIMORFIZM: draw() i update() nadpisują metody z GameObject
// ============================================================

Obstacle::Obstacle(sf::Vector2f pos, int type)
    : m_type(type)      // Zapamiętaj typ przeszkody
    , m_rotation(0.f)   // Kąt obrotu animacji od zera
{
    m_pos = pos;                          // Ustaw pozycję (pole z klasy bazowej)
    m_shape.setRadius(OBSTACLE_RADIUS);   // Promień kółka
    m_shape.setOrigin(OBSTACLE_RADIUS, OBSTACLE_RADIUS); // Środek obrotu

    m_shape.setPosition(pos); // Ustaw pozycję kształtu

    if (type == 0) {
        // Beczka – brązowa z ciemną obwódką
        m_shape.setFillColor(sf::Color(130, 70, 20));
        m_shape.setOutlineColor(sf::Color(60, 30, 5));
    } else {
        // Pachołek – pomarańczowy z ciemną obwódką
        m_shape.setFillColor(sf::Color(230, 100, 10));
        m_shape.setOutlineColor(sf::Color(100, 40, 0));
    }
    m_shape.setOutlineThickness(3.f);
}

// ============================================================
//  update – animacja obrotu przeszkody (POLIMORFIZM)
//  Nadpisuje metodę wirtualną z klasy bazowej GameObject
// ============================================================
void Obstacle::update(float dt)
{
    // Obracaj beczkę powoli dla efektu wizualnego
    m_rotation += 30.f * dt; // 30 stopni na sekundę
    if (m_rotation > 360.f) m_rotation -= 360.f;
    m_shape.setRotation(m_rotation); // Ustaw nowy kąt obrotu
}

// ============================================================
//  draw – rysuje przeszkodę (POLIMORFIZM)
//  Nadpisuje metodę czysto wirtualną z klasy bazowej GameObject
// ============================================================
void Obstacle::draw(sf::RenderWindow& window) const
{
    if (!m_active) return; // Nieaktywna – nie rysuj

    window.draw(m_shape); // Narysuj kółko

    // Narysuj pasy na przeszkodzie
    if (m_type == 0) {
        // Poziomy pasek beczki
        sf::RectangleShape stripe(sf::Vector2f(OBSTACLE_RADIUS * 1.8f, 4.f));
        stripe.setOrigin(OBSTACLE_RADIUS * 0.9f, 2.f);
        stripe.setPosition(m_pos);
        stripe.setRotation(m_rotation); // Obróć razem z beczką
        stripe.setFillColor(sf::Color(200, 160, 80));
        window.draw(stripe);
    } else {
        // Pionowy pasek pachołka
        sf::RectangleShape stripe(sf::Vector2f(5.f, OBSTACLE_RADIUS * 1.6f));
        stripe.setOrigin(2.5f, OBSTACLE_RADIUS * 0.8f);
        stripe.setPosition(m_pos);
        stripe.setFillColor(sf::Color(255, 255, 255, 200));
        window.draw(stripe);
    }
}

// ============================================================
//  checkCollision – detekcja kolizji kółko-kółko
// ============================================================
bool Obstacle::checkCollision(sf::Vector2f carPos) const
{
    float dx   = carPos.x - m_pos.x;
    float dy   = carPos.y - m_pos.y;
    float dist = std::sqrt(dx * dx + dy * dy);
    return dist < (OBSTACLE_RADIUS + CAR_WIDTH * 0.5f);
}

// Getter pozycji – nadpisuje metodę wirtualną z klasy bazowej
sf::Vector2f Obstacle::getPosition() const { return m_pos; }
