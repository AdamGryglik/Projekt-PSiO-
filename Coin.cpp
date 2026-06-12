#include "Coin.h"
#include <cmath>  // sqrt, sin

// ============================================================
//  Coin.cpp – implementacja monety
//  POLIMORFIZM: draw() i update() nadpisują metody z GameObject
// ============================================================

Coin::Coin(sf::Vector2f pos)
    : m_collected(false) // Moneta dostępna do zebrania
    , m_animTimer(0.f)   // Timer animacji od zera
    , m_scale(1.f)       // Normalny rozmiar na starcie
{
    m_pos = pos;                      // Ustaw pozycję (pole z klasy bazowej)
    m_shape.setRadius(COIN_RADIUS);   // Promień kółka monety
    m_shape.setOrigin(COIN_RADIUS, COIN_RADIUS); // Środek obrotu

    m_shape.setFillColor(sf::Color(255, 210, 0));    // Złoty kolor
    m_shape.setOutlineColor(sf::Color(180, 130, 0)); // Ciemnożółta obwódka
    m_shape.setOutlineThickness(2.f);
    m_shape.setPosition(pos);
}

// ============================================================
//  update – animacja pulsowania monety (POLIMORFIZM)
//  Nadpisuje metodę wirtualną z klasy bazowej GameObject
// ============================================================
void Coin::update(float dt)
{
    if (m_collected) return; // Zebrana – nie animuj

    // Pulsowanie – zmiana rozmiaru w rytm sinusa
    m_animTimer += dt * 3.f; // Prędkość pulsowania
    m_scale = 1.f + 0.15f * std::sin(m_animTimer); // Skala 0.85 .. 1.15

    // Zastosuj skalę do kształtu
    float r = COIN_RADIUS * m_scale;
    m_shape.setRadius(r);
    m_shape.setOrigin(r, r); // Utrzymaj środek
}

// ============================================================
//  draw – rysuje monetę z efektem połysku (POLIMORFIZM)
//  Nadpisuje metodę czysto wirtualną z klasy bazowej GameObject
// ============================================================
void Coin::draw(sf::RenderWindow& window) const
{
    if (m_collected) return; // Zebrana – nie rysuj

    window.draw(m_shape); // Narysuj złote kółko

    // Mały biały punkt połysku
    sf::CircleShape shine(3.f);
    shine.setOrigin(3.f, 3.f);
    shine.setPosition(m_pos + sf::Vector2f(-COIN_RADIUS * 0.4f, -COIN_RADIUS * 0.4f));
    shine.setFillColor(sf::Color(255, 255, 255, 180));
    window.draw(shine);
}

// ============================================================
//  tryCollect – próba zebrania monety
// ============================================================
bool Coin::tryCollect(sf::Vector2f carPos)
{
    if (m_collected) return false; // Już zebrana

    float dx   = carPos.x - m_pos.x;
    float dy   = carPos.y - m_pos.y;
    float dist = std::sqrt(dx * dx + dy * dy);

    if (dist < COIN_COLLECT_DIST) {
        m_collected = true; // Oznacz jako zebrana
        setActive(false);   // Dezaktywuj przez metodę klasy bazowej
        return true;
    }
    return false;
}

// Getter – czy zebrana
bool Coin::isCollected() const { return m_collected; }

// Getter pozycji – nadpisuje metodę wirtualną z klasy bazowej
sf::Vector2f Coin::getPosition() const { return m_pos; }
