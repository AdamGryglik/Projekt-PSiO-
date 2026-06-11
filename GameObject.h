#pragma once
#include <SFML/Graphics.hpp>
class GameObject{ // Abstrakcyjna klasa bazowa dla wszystkich obiektów gry
public:
  virtual ~GameObject() = default; // wirtualny destruktor, niszczący obiekty przez wskaźnik bazowy 
  virtual void draw(sf::RenderWindow& window) const = 0; // wirtualna metoda rysowania
  virtual void update(float dt) {}; // wirtualna metoda aktualizacji
  virtual sf::Vector2f getPosition() const = 0; // zwraca pozycję obiektu 
  virtual bool isActive() const{ return m_active; } // sprawdza czy obiekt jest aktywny
  virtual void setActive(bool active) {m_active = active;} // ustawia stan aktywności obiektu
protected:
  bool m_active = true; // Czy obiekt jest aktwyny
  sf::Vector2f m_pos; // Pozycja środka obiektu w pikselach 
};

  
