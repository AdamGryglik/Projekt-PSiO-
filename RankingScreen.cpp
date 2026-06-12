#include "RankingScreen.h"
#include "Constants.h"
#include <sstream>   // std::ostringstream
#include <iomanip>   // std::setw, std::setfill
#include <iostream>  // Komunikaty błędów

// ============================================================
//  Konstruktor
// ============================================================
RankingScreen::RankingScreen() : m_loaded(false)
{
    // Wczytaj czcionkę z popularnych lokalizacji
    m_loaded =
        m_font.loadFromFile("C:/Windows/Fonts/arial.ttf") ||
        m_font.loadFromFile("C:/Windows/Fonts/calibri.ttf") ||
        m_font.loadFromFile("/usr/share/fonts/truetype/liberation/LiberationMono-Regular.ttf") ||
        m_font.loadFromFile("/usr/share/fonts/truetype/freefont/FreeMono.ttf") ||
        m_font.loadFromFile("/usr/share/fonts/truetype/ubuntu/UbuntuMono-R.ttf") ||
        m_font.loadFromFile("/System/Library/Fonts/Helvetica.ttc");

    // Ciemne tło ekranu rankingu
    m_bg.setSize(sf::Vector2f((float)WINDOW_WIDTH, (float)WINDOW_HEIGHT));
    m_bg.setFillColor(sf::Color(15, 15, 30));
}

// ============================================================
//  handleEvent – ESC lub klik = powrót do menu
// ============================================================
bool RankingScreen::handleEvent(const sf::Event& event)
{
    // Klawisz ESC lub Enter = wróć
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Escape ||
            event.key.code == sf::Keyboard::Return) {
            return true; // Wróć do menu
        }
    }
    // Kliknięcie myszy = wróć
    if (event.type == sf::Event::MouseButtonPressed) {
        return true;
    }
    return false; // Zostań na ekranie rankingu
}

// ============================================================
//  draw – rysuje tabelę rankingową
// ============================================================
void RankingScreen::draw(sf::RenderWindow& window, const Ranking& ranking) const
{
    window.draw(m_bg); // Tło

    if (!m_loaded) return; // Bez fontu tylko tło

    float W = (float)WINDOW_WIDTH;

    // --- Nagłówek ---
    sf::Text title = makeText("RANKING - TOP 3", 32,
        sf::Color(255, 215, 0), // Złoty kolor tytułu
        sf::Vector2f(W * 0.5f - 140.f, 20.f));
    window.draw(title);

    // --- Instrukcja powrotu ---
    sf::Text back = makeText("Nacisnij ESC lub kliknij aby wrocic", 16,
        sf::Color(160, 160, 160),
        sf::Vector2f(W * 0.5f - 160.f, (float)WINDOW_HEIGHT - 30.f));
    window.draw(back);

    // Nazwy torów do wyświetlenia
    const char* trackNames[3] = {"TRAWA", "PIASEK", "MIASTO"};
    // Kolory nagłówków torów
    sf::Color   trackColors[3] = {
        sf::Color(80, 200, 80),   // Zielony = trawa
        sf::Color(230, 190, 60),  // Żółty = piasek
        sf::Color(150, 150, 220)  // Fioletowy = miasto
    };

    // --- Kolumny: każdy tor w osobnej kolumnie ---
    float colW = W / 3.f; // Szerokość kolumny
    float startY = 80.f;   // Y pierwszego wiersza

    for (int t = 0; t < 3; t++) {
        float colX = t * colW + 20.f; // X lewego marginesu kolumny

        // Nagłówek toru
        sf::Text torTitle = makeText(trackNames[t], 22,
            trackColors[t], sf::Vector2f(colX, startY));
        window.draw(torTitle);

        // --- Ranking czasów ---
        sf::Text timeHeader = makeText("Najlepsze czasy:", 15,
            sf::Color(200, 200, 200), sf::Vector2f(colX, startY + 35.f));
        window.draw(timeHeader);

        auto times = ranking.getTopTimes(t); // Pobierz top 3 czasy
        for (int r = 0; r < (int)times.size(); r++) {
            // Sformatuj czas MM:SS
            int totalSec = (int)times[r].timeSeconds;
            int mins = totalSec / 60;
            int secs = totalSec % 60;
            std::ostringstream oss;
            oss << (r + 1) << ". "
                << std::setfill('0') << std::setw(2) << mins << ":"
                << std::setfill('0') << std::setw(2) << secs;

            sf::Text timeText = makeText(oss.str(), 15,
                sf::Color::White, sf::Vector2f(colX + 10.f, startY + 55.f + r * 22.f));
            window.draw(timeText);
        }
        // Jeśli brak wyników
        if (times.empty()) {
            sf::Text noData = makeText("brak danych", 14,
                sf::Color(100, 100, 100), sf::Vector2f(colX + 10.f, startY + 55.f));
            window.draw(noData);
        }

        // --- Ranking monet ---
        sf::Text coinHeader = makeText("Najlepsze monety:", 15,
            sf::Color(255, 215, 0), sf::Vector2f(colX, startY + 160.f));
        window.draw(coinHeader);

        auto coins = ranking.getTopCoins(t); // Pobierz top 3 monety
        for (int r = 0; r < (int)coins.size(); r++) {
            std::ostringstream oss;
            oss << (r + 1) << ". " << coins[r].coins << " monet";

            sf::Text coinText = makeText(oss.str(), 15,
                sf::Color(255, 215, 0), sf::Vector2f(colX + 10.f, startY + 180.f + r * 22.f));
            window.draw(coinText);
        }
        // Jeśli brak wyników monet
        if (coins.empty()) {
            sf::Text noData = makeText("brak danych", 14,
                sf::Color(100, 100, 100), sf::Vector2f(colX + 10.f, startY + 180.f));
            window.draw(noData);
        }
    }
}

// ============================================================
//  makeText – pomocnicza do tworzenia sf::Text
// ============================================================
sf::Text RankingScreen::makeText(const std::string& str, unsigned size,
                                  sf::Color color, sf::Vector2f pos) const
{
    sf::Text t;
    t.setFont(m_font);           // Ustaw czcionkę
    t.setString(str);            // Ustaw treść
    t.setCharacterSize(size);    // Ustaw rozmiar
    t.setFillColor(color);       // Ustaw kolor
    t.setPosition(pos);          // Ustaw pozycję
    return t;
}
