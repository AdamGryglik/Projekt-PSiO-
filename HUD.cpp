#include "HUD.h"
#include "Constants.h"
#include <sstream>   // std::ostringstream – formatowanie tekstu
#include <iomanip>   // std::setfill, std::setw – wypełnianie zerami
#include <iostream>  // Komunikaty błędów

// ============================================================
//  Konstruktor – przygotowuje teksty HUD
// ============================================================
HUD::HUD() : m_loaded(false)
{
    // Wczytaj domyślny font systemowy (Ubuntu/Linux)
    // Próbujemy kilka popularnych lokalizacji czcionek
    if (m_font.loadFromFile("C:/Windows/Fonts/arial.ttf") ||    // Windows – pierwsza próba
        m_font.loadFromFile("C:/Windows/Fonts/calibri.ttf") ||  // Windows – druga próba
        m_font.loadFromFile("/usr/share/fonts/truetype/liberation/LiberationMono-Regular.ttf") ||
        m_font.loadFromFile("/usr/share/fonts/truetype/freefont/FreeMono.ttf") ||
        m_font.loadFromFile("/usr/share/fonts/truetype/ubuntu/UbuntuMono-R.ttf") ||
        m_font.loadFromFile("/System/Library/Fonts/Helvetica.ttc")) // macOS
    {
        m_loaded = true; // Font wczytany pomyślnie
    } else {
        std::cerr << "[HUD] Nie znaleziono fontu – tekst nie bedzie widoczny\n";
    }

    // --- Pasek tła HUD (ciemny, półprzezroczysty) ---
    m_bar.setSize(sf::Vector2f((float)WINDOW_WIDTH, 30.f)); // Pasek przez cały dół ekranu
    m_bar.setPosition(0.f, (float)WINDOW_HEIGHT - 30.f);    // Przyklejony do dołu
    m_bar.setFillColor(sf::Color(0, 0, 0, 160));            // Czarny, 63% przezroczysty

    if (!m_loaded) return; // Bez fontu nie konfiguruj tekstów

    // --- Wspólne ustawienia tekstów ---
    unsigned int charSize = 18; // Rozmiar czcionki w punktach

    // Tekst czasu – lewy dolny róg
    m_timeText.setFont(m_font);
    m_timeText.setCharacterSize(charSize);
    m_timeText.setFillColor(sf::Color::White);
    m_timeText.setPosition(10.f, (float)WINDOW_HEIGHT - 26.f);

    // Tekst okrążenia – środek lewej strony
    m_lapText.setFont(m_font);
    m_lapText.setCharacterSize(charSize);
    m_lapText.setFillColor(sf::Color(200, 255, 200)); // Jasnozielony
    m_lapText.setPosition(200.f, (float)WINDOW_HEIGHT - 26.f);

    // Tekst prędkości – środek paska
    m_speedText.setFont(m_font);
    m_speedText.setCharacterSize(charSize);
    m_speedText.setFillColor(sf::Color(200, 200, 255)); // Jasnoniebieski
    m_speedText.setPosition(420.f, (float)WINDOW_HEIGHT - 26.f);

    // Tekst punktów – prawy dolny róg
    m_coinText.setFont(m_font);
    m_coinText.setCharacterSize(charSize);
    m_coinText.setFillColor(sf::Color(255, 215, 0)); // Złoty
    m_coinText.setPosition(680.f, (float)WINDOW_HEIGHT - 26.f);
}

// ============================================================
//  update – aktualizuje wartości wyświetlane na HUD
// ============================================================
void HUD::update(float elapsedSeconds, float speedKmh, int lap, int totalLaps, int coins)
{
    if (!m_loaded) return; // Bez fontu nic nie robimy

    // Sformatuj czas jako "CZAS: MM:SS"
    m_timeText.setString("CZAS:" + formatTime(elapsedSeconds));

    // Sformatuj numer okrążenia "OKR: 2/3"
    m_lapText.setString("OKR:" + std::to_string(lap) + "/" + std::to_string(totalLaps));

    // Sformatuj prędkość "V: 120 km/h"
    m_speedText.setString("V:" + std::to_string((int)speedKmh) + "km/h");

    // Sformatuj punkty "PKT: 250"
    m_coinText.setString("PKT:" + std::to_string(coins));
}

// ============================================================
//  draw – rysuje pasek HUD na ekranie
// ============================================================
void HUD::draw(sf::RenderWindow& window) const
{
    window.draw(m_bar);       // Ciemne tło paska
    if (!m_loaded) return;    // Bez fontu nie rysuj tekstu
    window.draw(m_timeText);  // Czas
    window.draw(m_lapText);   // Okrążenie
    window.draw(m_speedText); // Prędkość
    window.draw(m_coinText);  // Punkty/monety
}

// ============================================================
//  formatTime – zamienia sekundy na "MM:SS"
// ============================================================
std::string HUD::formatTime(float seconds)
{
    int totalSec = (int)seconds;          // Zaokrągl do pełnych sekund
    int mins     = totalSec / 60;          // Minuty
    int secs     = totalSec % 60;          // Pozostałe sekundy

    std::ostringstream oss;
    oss << std::setfill('0') << std::setw(2) << mins  // 2 cyfry minut z zerem
        << ":"
        << std::setfill('0') << std::setw(2) << secs; // 2 cyfry sekund z zerem
    return oss.str();
}

bool HUD::isLoaded() const { return m_loaded; }
