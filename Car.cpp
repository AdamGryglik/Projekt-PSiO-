#include "Car.h"
#include <cmath>  // sin, cos, abs

// Przelicznik stopni na radiany
static const float DEG_TO_RAD = 3.14159265f / 180.f;

// ============================================================
//  Konstruktor – inicjalizuje auto
//  DZIEDZICZENIE: wywołuje konstruktor klasy bazowej GameObject
// ============================================================
Car::Car(sf::Vector2f startPos, sf::Color color)
    : m_angle(180.f)   // Auto startuje skierowane w lewo
    , m_speed(0.f)     // Prędkość zerowa na starcie
    , m_color(color)   // Kolor nadwozia
    , m_startPos(startPos) // Zapamiętaj pozycję startową
{
    m_pos = startPos; // Ustaw pozycję (pole z klasy bazowej GameObject)

    m_shape.setSize(sf::Vector2f(CAR_WIDTH, CAR_HEIGHT));
    m_shape.setOrigin(CAR_WIDTH / 2.f, CAR_HEIGHT / 2.f);
    m_shape.setFillColor(color);
    m_shape.setOutlineColor(sf::Color(20, 20, 20));
    m_shape.setOutlineThickness(2.f);
}

// ============================================================
//  update (override) – wersja bez wejścia (wymagana przez GameObject)
//  POLIMORFIZM – nadpisuje metodę wirtualną z klasy bazowej
// ============================================================
void Car::update(float dt)
{
    // Domyślna aktualizacja bez wejścia – tylko tarcie
    update(dt, false, false, false, false);
}

// ============================================================
//  update – wersja z wejściem klawiatury
// ============================================================
void Car::update(float dt, bool gasDown, bool brakeDown,
                 bool leftDown, bool rightDown)
{
    // --- Skręcanie ---
    float speedFactor = std::abs(m_speed) / CAR_MAX_SPEED;
    float turnAmount  = CAR_TURN_SPEED * speedFactor * dt;

    if (leftDown)  m_angle -= turnAmount; // Skręt w lewo
    if (rightDown) m_angle += turnAmount; // Skręt w prawo

    // --- Przyspieszenie / hamowanie z bezwładnością ---
    if (gasDown) {
        m_speed += CAR_ACCELERATION * dt; // Dodaj przyspieszenie
    } else if (brakeDown) {
        m_speed -= CAR_BRAKE_DECEL * dt;  // Hamowanie
    } else {
        // Pasywne tarcie – zwalnianie po puszczeniu gazu
        if (m_speed > 0.f) {
            m_speed -= CAR_FRICTION * dt;
            if (m_speed < 0.f) m_speed = 0.f;
        } else if (m_speed < 0.f) {
            m_speed += CAR_FRICTION * dt;
            if (m_speed > 0.f) m_speed = 0.f;
        }
    }

    // --- Ogranicz prędkość ---
    if (m_speed >  CAR_MAX_SPEED)   m_speed =  CAR_MAX_SPEED;
    if (m_speed < -CAR_MAX_REVERSE) m_speed = -CAR_MAX_REVERSE;

    // --- Oblicz wektor ruchu ---
    float rad = m_angle * DEG_TO_RAD;
    sf::Vector2f dir(std::cos(rad), std::sin(rad));
    m_pos += dir * m_speed * dt; // Przesuń pozycję

    // --- Zaktualizuj kształt SFML ---
    m_shape.setPosition(m_pos);
    m_shape.setRotation(m_angle + 90.f);
}

// ============================================================
//  draw (override) – rysuje auto (POLIMORFIZM)
//  Nadpisuje metodę czysto wirtualną z klasy bazowej GameObject
// ============================================================
void Car::draw(sf::RenderWindow& window) const
{
    window.draw(m_shape); // Narysuj nadwozie

    // Rysuj lusterka
    float rad = (m_angle + 90.f) * DEG_TO_RAD;
    sf::Vector2f side(std::cos(rad), std::sin(rad));
    float rad_fwd = m_angle * DEG_TO_RAD;
    sf::Vector2f fwd(std::cos(rad_fwd), std::sin(rad_fwd));

    sf::Vector2f mirrorL = m_pos + side * (CAR_WIDTH * 0.65f) + fwd * (CAR_HEIGHT * 0.25f);
    sf::Vector2f mirrorR = m_pos - side * (CAR_WIDTH * 0.65f) + fwd * (CAR_HEIGHT * 0.25f);

    sf::CircleShape mirror(4.f);
    mirror.setOrigin(4.f, 4.f);
    mirror.setFillColor(sf::Color(180, 180, 60));

    mirror.setPosition(mirrorL);
    window.draw(mirror);
    mirror.setPosition(mirrorR);
    window.draw(mirror);
}

// ============================================================
//  reset – przywróć auto do pozycji startowej
// ============================================================
void Car::reset(sf::Vector2f startPos)
{
    m_pos      = startPos;
    m_startPos = startPos;
    m_angle    = 180.f;  // Skieruj w lewo
    m_speed    = 0.f;
    m_shape.setPosition(m_pos);
    m_shape.setRotation(m_angle + 90.f);
}

// ============================================================
//  applyCollision – odbicie przy zderzeniu
// ============================================================
void Car::applyCollision()
{
    m_speed = -m_speed * CAR_COLLISION_BOUNCE;
    float rad = m_angle * DEG_TO_RAD;
    m_pos.x -= std::cos(rad) * 8.f;
    m_pos.y -= std::sin(rad) * 8.f;
    m_shape.setPosition(m_pos);
}

// ============================================================
//  Gettery
// ============================================================
sf::Vector2f  Car::getPosition() const { return m_pos; }
float         Car::getAngle()    const { return m_angle; }
float         Car::getSpeed()    const { return m_speed; }

sf::FloatRect Car::getBounds() const
{
    return m_shape.getGlobalBounds(); // AABB do detekcji kolizji
}

void Car::setPosition(sf::Vector2f pos)
{
    m_pos = pos;
    m_shape.setPosition(m_pos);
}
