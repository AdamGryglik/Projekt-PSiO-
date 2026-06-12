#include "EndScreen.h"
#include "Constants.h"
#include <sstream>   // std::ostringstream
#include <iomanip>   // std::setw, std::setfill

// ============================================================
//  Konstruktor
// ============================================================
EndScreen::EndScreen()
    : m_loaded(false), m_time(0.f), m_coins(0), m_record(false)
{
    // Wczytaj czcionkę
    m_loaded =
        m_font.loadFromFile("C:/Windows/Fonts/arial.ttf") ||
        m_font.loadFromFile("C:/Windows/Fonts/calibri.ttf") ||
        m_font.loadFromFile("/usr/share/fonts/truetype/liberation/LiberationMono-Regular.ttf") ||
        m_font.loadFromFile("/usr/share/fonts/truetype/freefont/FreeMono.ttf") ||
        m_font.loadFromFile("/usr/share/fonts/truetype/ubuntu/UbuntuMono-R.ttf") ||
        m_font.loadFromFile("/System/Library/Fonts/Helvetica.ttc");

    float W = (float)WINDOW_WIDTH;
    float H = (float)WINDOW_HEIGHT;

    // Ciemne tło z zielonkawym odcieniem (sukces wyścigu)
    m_bg.setSize(sf::Vector2f(W, H));
    m_bg.setFillColor(sf::Color(10, 25, 10));

    // --- Przycisk "MENU GŁÓWNE" ---
    m_menuBtn.setSize(sf::Vector2f(220.f, 55.f));
    m_menuBtn.setPosition(W * 0.5f - 240.f, H - 100.f);
    m_menuBtn.setFillColor(sf::Color(60, 60, 160));  // Niebieski
    m_menuBtn.setOutlineColor(sf::Color(100, 100, 220));
    m_menuBtn.setOutlineThickness(3.f);

    // --- Przycisk "RANKING" ---
    m_rankBtn.setSize(sf::Vector2f(220.f, 55.f));
    m_rankBtn.setPosition(W * 0.5f + 20.f, H - 100.f);
    m_rankBtn.setFillColor(sf::Color(150, 100, 20));  // Złoto-brązowy
    m_rankBtn.setOutlineColor(sf::Color(255, 200, 50));
    m_rankBtn.setOutlineThickness(3.f);

    if (m_loaded) {
        // Tekst przycisku menu
        m_menuBtnText.setFont(m_font);
        m_menuBtnText.setString("MENU GLOWNE");
        m_menuBtnText.setCharacterSize(20);
        m_menuBtnText.setFillColor(sf::Color::White);
        m_menuBtnText.setPosition(W * 0.5f - 225.f, H - 85.f);

        // Tekst przycisku rankingu
        m_rankBtnText.setFont(m_font);
        m_rankBtnText.setString("RANKING");
        m_rankBtnText.setCharacterSize(20);
        m_rankBtnText.setFillColor(sf::Color::White);
        m_rankBtnText.setPosition(W * 0.5f + 55.f, H - 85.f);
    }
}

// ============================================================
//  setResults – aktualizuje wyniki wyświetlane na ekranie
// ============================================================
void EndScreen::setResults(float timeSec, int coins, bool isRecord)
{
    m_time   = timeSec;   // Zapisz czas wyścigu
    m_coins  = coins;     // Zapisz liczbę monet
    m_record = isRecord;  // Czy to rekord
}

// ============================================================
//  handleEvent – obsługa kliknięć
// ============================================================
EndChoice EndScreen::handleEvent(const sf::Event& event)
{
    if (event.type == sf::Event::MouseButtonPressed &&
        event.mouseButton.button == sf::Mouse::Left)
    {
        sf::Vector2f click((float)event.mouseButton.x, (float)event.mouseButton.y);

        if (m_menuBtn.getGlobalBounds().contains(click))
            return EndChoice::Menu; // Kliknięto "Menu główne"

        if (m_rankBtn.getGlobalBounds().contains(click))
            return EndChoice::Ranking; // Kliknięto "Ranking"
    }

    // Klawisz M = menu, R = ranking, ESC = menu
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Escape ||
            event.key.code == sf::Keyboard::M)
            return EndChoice::Menu;
        if (event.key.code == sf::Keyboard::R)
            return EndChoice::Ranking;
    }

    return EndChoice::None; // Jeszcze żaden wybór
}

// ============================================================
//  draw – rysuje ekran podsumowania
// ============================================================
void EndScreen::draw(sf::RenderWindow& window) const
{
    window.draw(m_bg); // Tło

    if (!m_loaded) return; // Bez fontu tylko tło

    float W = (float)WINDOW_WIDTH;

    // --- Tytuł "WYŚCIG UKOŃCZONY!" ---
    sf::Text title = makeText("WYSCIG UKONCZONY!", 36,
        sf::Color(100, 255, 100), sf::Vector2f(W * 0.5f - 175.f, 60.f));
    window.draw(title);

    // --- Czas wyścigu ---
    int totalSec = (int)m_time;
    int mins = totalSec / 60;
    int secs = totalSec % 60;
    std::ostringstream timeOss;
    timeOss << "Czas: "
            << std::setfill('0') << std::setw(2) << mins << ":"
            << std::setfill('0') << std::setw(2) << secs;

    sf::Text timeText = makeText(timeOss.str(), 28,
        sf::Color::White, sf::Vector2f(W * 0.5f - 80.f, 160.f));
    window.draw(timeText);

    // --- Liczba zebranych monet ---
    sf::Text coinText = makeText("Zebrane monety: " + std::to_string(m_coins), 28,
        sf::Color(255, 215, 0), sf::Vector2f(W * 0.5f - 130.f, 210.f));
    window.draw(coinText);

    // --- Komunikat o rekordzie ---
    if (m_record) {
        sf::Text recordText = makeText("NOWY REKORD!", 30,
            sf::Color(255, 50, 50), sf::Vector2f(W * 0.5f - 100.f, 270.f));
        window.draw(recordText);
    }

    // --- Instrukcja klawiszy ---
    sf::Text hint = makeText("M = Menu  |  R = Ranking  |  ESC = Menu", 16,
        sf::Color(140, 140, 140), sf::Vector2f(W * 0.5f - 170.f, 350.f));
    window.draw(hint);

    // --- Przyciski ---
    window.draw(m_menuBtn);
    window.draw(m_rankBtn);
    window.draw(m_menuBtnText);
    window.draw(m_rankBtnText);
}

// ============================================================
//  makeText – pomocnicza tworzenia sf::Text
// ============================================================
sf::Text EndScreen::makeText(const std::string& str, unsigned size,
                              sf::Color color, sf::Vector2f pos) const
{
    sf::Text t;
    t.setFont(m_font);
    t.setString(str);
    t.setCharacterSize(size);
    t.setFillColor(color);
    t.setPosition(pos);
    return t;
}
