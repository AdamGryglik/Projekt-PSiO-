#include "Menu.h"
#include "Constants.h"
#include <iostream>  // Komunikaty błędów

// Kolory przycisków wybranych i niewybranych
const sf::Color Menu::SEL_COLOR   = sf::Color(60, 120, 220);  // Niebieski = zaznaczony
const sf::Color Menu::UNSEL_COLOR = sf::Color(50, 50, 70);    // Ciemnoszary = niezaznaczony

// ============================================================
//  Konstruktor
// ============================================================
Menu::Menu()
    : m_loaded(false)
    , m_selectedCar(0)   // Domyślnie czerwone auto
    , m_selectedTrack(0) // Domyślnie tor trawiasty
{
    m_loaded = loadFont(); // Wczytaj czcionkę
    setupUI();             // Zbuduj elementy interfejsu
}

// ============================================================
//  loadFont – próbuje wczytać czcionkę z różnych lokalizacji
// ============================================================
bool Menu::loadFont()
{
    return m_font.loadFromFile("C:/Windows/Fonts/arial.ttf") ||    // Windows – pierwsza próba
           m_font.loadFromFile("C:/Windows/Fonts/calibri.ttf") ||  // Windows – druga próba
           m_font.loadFromFile("/usr/share/fonts/truetype/liberation/LiberationMono-Regular.ttf") ||
           m_font.loadFromFile("/usr/share/fonts/truetype/freefont/FreeMono.ttf") ||
           m_font.loadFromFile("/usr/share/fonts/truetype/ubuntu/UbuntuMono-R.ttf") ||
           m_font.loadFromFile("/System/Library/Fonts/Helvetica.ttc");
}

// ============================================================
//  setupUI – inicjalizuje wszystkie elementy ekranu menu
// ============================================================
void Menu::setupUI()
{
    float W = (float)WINDOW_WIDTH;   // 900
    float H = (float)WINDOW_HEIGHT;  // 600

    // --- Tło menu ---
    m_bg.setSize(sf::Vector2f(W, H));
    m_bg.setFillColor(sf::Color(20, 20, 35)); // Granatowe tło

    // --- Tytuł "WYBIERZ AUTO" ---
    if (m_loaded) {
        m_titleCar.setFont(m_font);
        m_titleCar.setString("WYBIERZ AUTO");
        m_titleCar.setCharacterSize(26);
        m_titleCar.setFillColor(sf::Color::White);
        m_titleCar.setPosition(W * 0.5f - 100.f, 40.f); // Wyśrodkuj

        // --- Tytuł "WYBIERZ TOR" ---
        m_titleTrack.setFont(m_font);
        m_titleTrack.setString("WYBIERZ TOR");
        m_titleTrack.setCharacterSize(26);
        m_titleTrack.setFillColor(sf::Color::White);
        m_titleTrack.setPosition(W * 0.5f - 95.f, 230.f);
    }

    // --- Ramki wyboru aut (dwa prostokąty obok siebie) ---
    float carBoxW = 160.f, carBoxH = 140.f;          // Rozmiar ramki auta
    float carY    = 90.f;                              // Pozycja Y ramek
    float car1X   = W * 0.5f - carBoxW - 20.f;       // Lewy (czerwone)
    float car2X   = W * 0.5f + 20.f;                  // Prawy (niebieskie)

    for (int i = 0; i < 2; i++) {
        float bx = (i == 0) ? car1X : car2X;          // Pozycja X tej ramki
        m_carBox[i].setSize(sf::Vector2f(carBoxW, carBoxH));
        m_carBox[i].setPosition(bx, carY);
        m_carBox[i].setOutlineThickness(3.f);
        m_carBox[i].setFillColor(sf::Color(40, 40, 60)); // Ciemne wnętrze

        // Miniaturka auta wewnątrz ramki
        m_carShape[i].setSize(sf::Vector2f(CAR_WIDTH * 2.f, CAR_HEIGHT * 2.f)); // 2x większe
        m_carShape[i].setOrigin(CAR_WIDTH, CAR_HEIGHT);
        m_carShape[i].setPosition(bx + carBoxW * 0.5f, carY + carBoxH * 0.5f - 10.f);
        m_carShape[i].setFillColor(i == 0
            ? sf::Color(CAR1_R, CAR1_G, CAR1_B)   // Czerwone auto
            : sf::Color(CAR2_R, CAR2_G, CAR2_B)); // Niebieskie auto
        m_carShape[i].setOutlineColor(sf::Color(20, 20, 20));
        m_carShape[i].setOutlineThickness(2.f);
    }

    // --- Przyciski wyboru toru (3 przyciski poziomo) ---
    float tBtnW = 210.f, tBtnH = 60.f; // Rozmiar przycisku toru
    float tY    = 280.f;                // Pozycja Y
    const char* trackNames[3] = {"TRAWA", "PIASEK", "MIASTO"}; // Etykiety
    sf::Color   trackColors[3] = {
        sf::Color(80, 140, 50),  // Zielony = trawa
        sf::Color(200, 160, 60), // Żółty = piasek
        sf::Color(80, 80, 100)   // Szary = miasto
    };

    float totalWidth = 3 * tBtnW + 2 * 20.f; // Całkowita szerokość 3 przycisków
    float tStartX = W * 0.5f - totalWidth * 0.5f; // X pierwszego przycisku

    for (int i = 0; i < 3; i++) {
        m_trackBtn[i].setSize(sf::Vector2f(tBtnW, tBtnH));
        m_trackBtn[i].setPosition(tStartX + i * (tBtnW + 20.f), tY);
        m_trackBtn[i].setFillColor(trackColors[i]);
        m_trackBtn[i].setOutlineThickness(3.f);

        if (m_loaded) {
            m_trackLabel[i].setFont(m_font);
            m_trackLabel[i].setString(trackNames[i]);
            m_trackLabel[i].setCharacterSize(20);
            m_trackLabel[i].setFillColor(sf::Color::White);
            // Wyśrodkuj tekst w przycisku
            m_trackLabel[i].setPosition(
                tStartX + i * (tBtnW + 20.f) + tBtnW * 0.5f - 35.f,
                tY + tBtnH * 0.5f - 12.f
            );
        }
    }

    // --- Przycisk START ---
    m_startBtn.setSize(sf::Vector2f(200.f, 60.f));
    m_startBtn.setPosition(W * 0.5f - 100.f, H - 110.f); // Dolna część ekranu
    m_startBtn.setFillColor(sf::Color(40, 160, 40));      // Zielony
    m_startBtn.setOutlineColor(sf::Color(20, 80, 20));
    m_startBtn.setOutlineThickness(3.f);

    if (m_loaded) {
        m_startText.setFont(m_font);
        m_startText.setString("START!");
        m_startText.setCharacterSize(28);
        m_startText.setFillColor(sf::Color::White);
        m_startText.setPosition(W * 0.5f - 42.f, H - 100.f);
    }

    // Odśwież wygląd zaznaczenia
    reset();
}

// ============================================================
//  handleEvent – obsługuje kliknięcia myszy i klawisz Enter
// ============================================================
bool Menu::handleEvent(const sf::Event& event, sf::RenderWindow& window)
{
    if (event.type == sf::Event::MouseButtonPressed &&
        event.mouseButton.button == sf::Mouse::Left)
    {
        // Przelicz pozycję kliknięcia na współrzędne okna
        sf::Vector2f click((float)event.mouseButton.x, (float)event.mouseButton.y);

        // Sprawdź kliknięcie w ramkę auta
        for (int i = 0; i < 2; i++) {
            if (m_carBox[i].getGlobalBounds().contains(click)) {
                m_selectedCar = i; // Zmień wybrane auto
                reset();           // Odśwież wygląd
            }
        }

        // Sprawdź kliknięcie w przycisk toru
        for (int i = 0; i < 3; i++) {
            if (m_trackBtn[i].getGlobalBounds().contains(click)) {
                m_selectedTrack = i; // Zmień wybrany tor
                reset();              // Odśwież wygląd
            }
        }

        // Sprawdź kliknięcie START
        if (m_startBtn.getGlobalBounds().contains(click)) {
            return true; // Gracz chce zacząć grę
        }
    }

    // Enter też startuje grę
    if (event.type == sf::Event::KeyPressed &&
        event.key.code == sf::Keyboard::Return) {
        return true;
    }

    return false; // Menu nadal aktywne
}

// ============================================================
//  draw – rysuje cały ekran menu
// ============================================================
void Menu::draw(sf::RenderWindow& window) const
{
    window.draw(m_bg);         // Tło

    // Rysuj ramki i miniaturki aut
    for (int i = 0; i < 2; i++) {
        window.draw(m_carBox[i]);
        window.draw(m_carShape[i]);
    }

    // Rysuj przyciski torów
    for (int i = 0; i < 3; i++) {
        window.draw(m_trackBtn[i]);
        if (m_loaded) window.draw(m_trackLabel[i]);
    }

    window.draw(m_startBtn); // Przycisk start
    if (m_loaded) {
        window.draw(m_titleCar);
        window.draw(m_titleTrack);
        window.draw(m_startText);
    }
}

// ============================================================
//  reset – odśwież kolory zaznaczenia po zmianie wyboru
// ============================================================
void Menu::reset()
{
    // Obwódka zaznaczonej ramki auta = niebieska, reszta szara
    for (int i = 0; i < 2; i++) {
        m_carBox[i].setOutlineColor(i == m_selectedCar
            ? sf::Color(60, 200, 255)   // Zaznaczone – jasnoniebieski
            : sf::Color(80, 80, 80));   // Niezaznaczone – szary
    }

    // Obwódka zaznaczonego toru = biała, reszta szara
    for (int i = 0; i < 3; i++) {
        m_trackBtn[i].setOutlineColor(i == m_selectedTrack
            ? sf::Color::White         // Zaznaczony – biały
            : sf::Color(30, 30, 30));  // Niezaznaczony – czarny
    }
}

// Gettery
int Menu::getSelectedCar()   const { return m_selectedCar; }
int Menu::getSelectedTrack() const { return m_selectedTrack; }
