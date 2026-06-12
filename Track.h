#pragma once

#include <SFML/Graphics.hpp>  // Grafika SFML
#include <vector>              // std::vector – lista punktów trasy
#include "Constants.h"         // Stałe gry

// ============================================================
//  Track.h – klasa toru wyścigowego
//  Tor to zamknięta pętla zdefiniowana przez listę punktów środkowych.
//  Rysuje tło, nawierzchnię, linie boczne i linię startową.
// ============================================================

class Track {
public:
    // Konstruktor – tworzy tor o podanym ID (0=trawa, 1=piasek, 2=miasto)
    explicit Track(int trackId);

    // Rysuje pełny tor (tło + nawierzchnia + metka)
    void draw(sf::RenderWindow& window) const;

    // Sprawdza czy podana pozycja jest na nawierzchni toru (nie poza)
    bool isOnTrack(sf::Vector2f pos) const;

    // Zwraca pozycję startową auta na tym torze
    sf::Vector2f getStartPosition() const;

    // Zwraca ID toru
    int getId() const;

    // Zwraca wektor punktów checkpointów do wykrywania okrążeń
    const std::vector<sf::FloatRect>& getCheckpoints() const;

    // Zwraca prostokąt linii startowej (do detekcji przekroczenia)
    sf::FloatRect getStartLine() const;

    // Zwraca kolor tła (trawa/piasek/miasto)
    sf::Color getBackgroundColor() const;

    // Jeśli pos jest poza torem – przesuwa ją z powrotem na krawędź jezdni
    // Zwraca true jeśli pozycja była poza torem (nastąpiło pchnięcie)
    bool pushBackOnTrack(sf::Vector2f& pos) const;

private:
    int m_trackId;                          // Identyfikator toru
    std::vector<sf::Vector2f> m_waypoints;  // Punkty środkowe trasy w kolejności
    float m_trackWidth;                     // Szerokość pasa jezdni w pikselach

    std::vector<sf::FloatRect> m_checkpoints; // Prostokąty do detekcji przejazdu
    sf::FloatRect m_startLine;                // Prostokąt linii startu/mety

    // Kolory nawierzchni i tła zależne od toru
    sf::Color m_surfaceColor;   // Kolor asfaltu
    sf::Color m_bgColor;        // Kolor tła (trawa / piasek / chodnik)
    sf::Color m_lineColor;      // Kolor linii bocznych

    // Buforowane VertexArrays do szybkiego rysowania
    mutable sf::VertexArray m_trackVA;  // Quad strip nawierzchni
    mutable bool m_dirty;               // Czy trzeba przebudować VA

    // Zbuduj VertexArray na podstawie waypoints
    void buildMesh() const;

    // Zainicjalizuj punkty trasy i checkpointy dla danego ID
    void initTrack();
};
