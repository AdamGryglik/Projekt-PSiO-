#include "Track.h"
#include <cmath>   // sin, cos, sqrt
#include <array>   // std::array

static const float PI = 3.14159265f;

// ============================================================
//  Konstruktor – wybiera schemat kolorów i inicjalizuje trasę
// ============================================================
Track::Track(int trackId)
    : m_trackId(trackId)
    , m_trackWidth(110.f) // Szerokość pasa jezdni – zwiększona dla komfortu jazdy
    , m_dirty(true)      // Siatka wymaga przebudowania
{
    // --- Wybierz kolory w zależności od toru ---
    switch (trackId) {
        case TRACK_GRASS: // Tor trawiasty – zielone tło, szary asfalt
            m_bgColor      = sf::Color(80,  160,  50);
            m_surfaceColor = sf::Color(90,  90,   90);
            m_lineColor    = sf::Color(230, 230, 230);
            break;
        case TRACK_SAND:  // Tor piaszczysty – pomarańczowe tło, beżowy asfalt
            m_bgColor      = sf::Color(220, 180,  80);
            m_surfaceColor = sf::Color(160, 140,  80);
            m_lineColor    = sf::Color(255, 255, 200);
            break;
        case TRACK_CITY:  // Tor miejski – ciemne tło, ciemny asfalt
            m_bgColor      = sf::Color(100, 100, 120);
            m_surfaceColor = sf::Color(60,  60,   70);
            m_lineColor    = sf::Color(255, 220,   0);
            break;
        default:
            m_bgColor      = sf::Color(80, 160, 50);
            m_surfaceColor = sf::Color(90, 90,  90);
            m_lineColor    = sf::Color(230,230,230);
    }

    initTrack(); // Zainicjalizuj punkty trasy
}

// ============================================================
//  initTrack – definiuje waypoints i checkpointy dla każdego toru
//  Wszystkie tory to owale/prostokąty z zaokrąglonymi narożnikami
// ============================================================
void Track::initTrack()
{
    m_waypoints.clear();
    m_checkpoints.clear();

    float W = (float)WINDOW_WIDTH;  // 900
    float H = (float)WINDOW_HEIGHT; // 600

    if (m_trackId == TRACK_GRASS) {
        // --- Tor 0: Trawa – owalny tor eliptyczny ---
        float cx = W * 0.5f;  // Centrum X = 450
        float cy = H * 0.5f;  // Centrum Y = 300
        float rx = W * 0.38f; // Promień poziomy = 342
        float ry = H * 0.36f; // Promień pionowy = 216
        int   N  = 32;         // Liczba punktów trasy

        // Generuj punkty na elipsie
        for (int i = 0; i < N; i++) {
            float t = (float)i / (float)N * 2.f * PI; // Kąt od 0 do 2π
            m_waypoints.push_back({
                cx + rx * std::cos(t), // Pozycja X na elipsie
                cy + ry * std::sin(t)  // Pozycja Y na elipsie
            });
        }

        // Linia startowa – pionowy pasek na prawej stronie górnego łuku
        // FloatRect(x, y, szerokość, wysokość) – wąski pionowy pasek
        m_startLine = sf::FloatRect(cx - 20.f, cy - ry - m_trackWidth*0.5f, 40.f, m_trackWidth);

        // Checkpointy co 90° toru (3 punkty kontrolne)
        m_checkpoints = {
            sf::FloatRect(cx + rx - 20.f, cy - 80.f, 40.f, 160.f),  // Prawo
            sf::FloatRect(cx - 80.f, cy + ry - 20.f, 160.f, 40.f),   // Dół
            sf::FloatRect(cx - rx - 20.f, cy - 80.f, 40.f, 160.f),  // Lewo
        };

    } else if (m_trackId == TRACK_SAND) {
        // --- Tor 1: Piasek – prostokątny tor z zaokrąglonymi narożnikami ---
        float margin = 80.f; // Margines od krawędzi okna
        float rx = W * 0.37f; // Połowa szerokości prostokąta
        float ry = H * 0.34f; // Połowa wysokości prostokąta
        float cx = W * 0.5f;
        float cy = H * 0.5f;
        int   arc = 8; // Punkty na każdym zaokrągleniu narożnika

        // Generuj punkty "zaokrąglonego prostokąta" (stadium shape)
        // Górna prawa krzywa
        for (int i = 0; i <= arc; i++) {
            float t = -PI * 0.5f + (float)i / arc * PI * 0.5f;
            m_waypoints.push_back({cx + rx * std::cos(t), cy - ry * std::sin(t) + ry - ry});
        }
        // Tutaj uproszczamy do 4 prostych odcinków + 4 łuki
        m_waypoints.clear();
        int   N  = 36;
        for (int i = 0; i < N; i++) {
            float t = (float)i / (float)N * 2.f * PI;
            // Superelipsa (n=4) daje zaokrąglony prostokąt
            float cosT = std::cos(t);
            float sinT = std::sin(t);
            m_waypoints.push_back({
                cx + rx * std::pow(std::abs(cosT), 0.55f) * (cosT > 0 ? 1.f : -1.f),
                cy + ry * std::pow(std::abs(sinT), 0.55f) * (sinT > 0 ? 1.f : -1.f)
            });
        }

        m_startLine  = sf::FloatRect(cx - 20.f, cy - ry - m_trackWidth*0.5f, 40.f, m_trackWidth);
        m_checkpoints = {
            sf::FloatRect(cx + rx - 20.f, cy - 80.f, 40.f, 160.f),
            sf::FloatRect(cx - 80.f, cy + ry - 20.f, 160.f, 40.f),
            sf::FloatRect(cx - rx - 20.f, cy - 80.f, 40.f, 160.f),
        };

    } else {
        // --- Tor 2: Miasto – bardziej kwadratowy, ostrzejsze zakręty ---
        float cx = W * 0.5f;
        float cy = H * 0.5f;
        float rx = W * 0.40f;
        float ry = H * 0.38f;
        int   N  = 28;

        for (int i = 0; i < N; i++) {
            float t = (float)i / (float)N * 2.f * PI;
            // Zaokrąglony prostokąt ze współczynnikiem n=6 (ostrzejszy)
            float cosT = std::cos(t);
            float sinT = std::sin(t);
            m_waypoints.push_back({
                cx + rx * std::pow(std::abs(cosT), 0.4f) * (cosT > 0 ? 1.f : -1.f),
                cy + ry * std::pow(std::abs(sinT), 0.4f) * (sinT > 0 ? 1.f : -1.f)
            });
        }

        m_startLine  = sf::FloatRect(cx - 20.f, cy - ry - m_trackWidth*0.5f, 40.f, m_trackWidth);
        m_checkpoints = {
            sf::FloatRect(cx + rx - 20.f, cy - 80.f, 40.f, 160.f),
            sf::FloatRect(cx - 80.f, cy + ry - 20.f, 160.f, 40.f),
            sf::FloatRect(cx - rx - 20.f, cy - 80.f, 40.f, 160.f),
        };
    }
}

// ============================================================
//  buildMesh – tworzy VertexArray paska wstęgowego dla jezdni
//  Dla każdej pary sąsiednich waypoints generuje quad po obu stronach
// ============================================================
void Track::buildMesh() const
{
    int N = (int)m_waypoints.size();
    if (N < 2) return;

    // Quad strip: 2 wierzchołki na punkt trasy (lewy + prawy skraj jezdni)
    m_trackVA.setPrimitiveType(sf::TrianglesStrip);
    m_trackVA.resize((N + 1) * 2); // +1 żeby zamknąć pętlę

    for (int i = 0; i <= N; i++) {
        int   cur  = i % N;        // Aktualny waypoint (modulo zamknięcia)
        int   next = (i + 1) % N;  // Następny waypoint
        sf::Vector2f p = m_waypoints[cur];
        sf::Vector2f q = m_waypoints[next];

        // Oblicz wektor prostopadły do kierunku jezdni (normala)
        sf::Vector2f dir = q - p;                    // Wektor wzdłuż jezdni
        float len = std::sqrt(dir.x*dir.x + dir.y*dir.y); // Długość wektora
        if (len > 0.f) dir /= len;                   // Znormalizuj
        sf::Vector2f normal(-dir.y, dir.x);           // Obróć o 90° = normala

        // Wierzchołki lewego i prawego skraju jezdni
        m_trackVA[i * 2 + 0].position = p - normal * (m_trackWidth * 0.5f);
        m_trackVA[i * 2 + 1].position = p + normal * (m_trackWidth * 0.5f);

        // Nadaj kolor nawierzchni
        m_trackVA[i * 2 + 0].color = m_surfaceColor;
        m_trackVA[i * 2 + 1].color = m_surfaceColor;
    }

    m_dirty = false; // Siatka jest aktualna
}

// ============================================================
//  draw – rysuje tło, jezdnię, linie boczne i linię startową
// ============================================================
void Track::draw(sf::RenderWindow& window) const
{
    // --- 1. Tło (wypełnij cały ekran kolorem toru) ---
    sf::RectangleShape bg(sf::Vector2f((float)WINDOW_WIDTH, (float)WINDOW_HEIGHT));
    bg.setFillColor(m_bgColor); // Zielony / piasek / szary
    window.draw(bg);

    // --- 2. Przebuduj siatkę jeśli trzeba ---
    if (m_dirty) buildMesh();

    // --- 3. Narysuj jezdnię (quad strip) ---
    window.draw(m_trackVA);

    // --- 4. Linie boczne toru (biała/żółta obwódka) ---
    int N = (int)m_waypoints.size();
    for (int side = -1; side <= 1; side += 2) { // -1 = lewa strona, +1 = prawa
        sf::VertexArray lineVA(sf::LineStrip, N + 1);
        for (int i = 0; i <= N; i++) {
            int  cur  = i % N;
            int  next = (i + 1) % N;
            sf::Vector2f p   = m_waypoints[cur];
            sf::Vector2f q   = m_waypoints[next];
            sf::Vector2f dir = q - p;
            float len = std::sqrt(dir.x*dir.x + dir.y*dir.y);
            if (len > 0.f) dir /= len;
            sf::Vector2f normal(-dir.y, dir.x);
            lineVA[i].position = p + normal * (float)side * (m_trackWidth * 0.5f - 3.f);
            lineVA[i].color    = m_lineColor;
        }
        window.draw(lineVA);
    }

    // --- 5. Linia startowa (biało-czarna szachownica – pionowa) ---
    // Linia startowa jest pionowa: szerokość mała, wysokość = szerokość toru
    int segments = 8; // Liczba kwadratów szachownicy wzdłuż wysokości
    float segH   = m_startLine.height / (float)segments; // Wysokość jednego kwadratu
    float segW   = m_startLine.width;                    // Szerokość = cała szerokość linii
    for (int i = 0; i < segments; i++) {
        sf::RectangleShape seg(sf::Vector2f(segW, segH));
        seg.setPosition(m_startLine.left, m_startLine.top + i * segH);
        // Na przemian biały i czarny
        seg.setFillColor(i % 2 == 0 ? sf::Color::White : sf::Color::Black);
        window.draw(seg);
    }
}

// ============================================================
//  isOnTrack – sprawdza czy punkt jest na nawierzchni jezdni
//  Szuka najbliższego waypointa i mierzy odległość od osi trasy
// ============================================================
bool Track::isOnTrack(sf::Vector2f pos) const
{
    float minDist = 1e9f; // Bardzo duża wartość startowa
    int N = (int)m_waypoints.size();

    for (int i = 0; i < N; i++) {
        // Odcinek między waypoint[i] a waypoint[i+1]
        sf::Vector2f a = m_waypoints[i];
        sf::Vector2f b = m_waypoints[(i + 1) % N];
        sf::Vector2f ab = b - a;                    // Wektor odcinka
        sf::Vector2f ap = pos - a;                  // Wektor od a do punktu
        float len2 = ab.x*ab.x + ab.y*ab.y;         // Kwadrat długości odcinka
        float t    = (len2 > 0.f) ? (ap.x*ab.x + ap.y*ab.y) / len2 : 0.f; // Parametr rzutu
        t = std::max(0.f, std::min(1.f, t));         // Ogranicz do [0,1]
        sf::Vector2f proj = a + t * ab;              // Punkt na odcinku najbliższy pos
        sf::Vector2f diff = pos - proj;
        float dist = std::sqrt(diff.x*diff.x + diff.y*diff.y); // Odległość od osi
        if (dist < minDist) minDist = dist;
    }

    // Punkt jest na torze jeśli bliżej niż połowa szerokości jezdni
    return minDist < m_trackWidth * 0.5f;
}

// ============================================================
//  Gettery
// ============================================================
sf::Vector2f Track::getStartPosition() const
{
    // Pozycja startowa auta = środek linii startowej, lekko poniżej
    return sf::Vector2f(
        m_startLine.left + m_startLine.width  * 0.5f,
        m_startLine.top  + m_startLine.height * 0.5f + 25.f
    );
}

int Track::getId() const { return m_trackId; }

const std::vector<sf::FloatRect>& Track::getCheckpoints() const { return m_checkpoints; }

sf::FloatRect Track::getStartLine() const { return m_startLine; }

sf::Color Track::getBackgroundColor() const { return m_bgColor; }

// ============================================================
//  pushBackOnTrack – przesuwa pozycję z powrotem na tor jeśli wyjechała poza
//  Szuka najbliższego punktu na osi trasy i przesuwa pos na krawędź jezdni
// ============================================================
bool Track::pushBackOnTrack(sf::Vector2f& pos) const
{
    float minDist = 1e9f;         // Najlepsza (minimalna) odległość od osi
    sf::Vector2f bestProj = pos;  // Najbliższy punkt na osi trasy
    int N = (int)m_waypoints.size();

    for (int i = 0; i < N; i++) {
        sf::Vector2f a = m_waypoints[i];
        sf::Vector2f b = m_waypoints[(i + 1) % N];
        sf::Vector2f ab = b - a;
        sf::Vector2f ap = pos - a;
        float len2 = ab.x*ab.x + ab.y*ab.y;
        float t    = (len2 > 0.f) ? (ap.x*ab.x + ap.y*ab.y) / len2 : 0.f;
        t = std::max(0.f, std::min(1.f, t));
        sf::Vector2f proj = a + t * ab;  // Rzut pos na odcinek trasy
        sf::Vector2f diff = pos - proj;
        float dist = std::sqrt(diff.x*diff.x + diff.y*diff.y);
        if (dist < minDist) {
            minDist   = dist;
            bestProj  = proj; // Zapamiętaj najbliższy punkt na osi
        }
    }

    float half = m_trackWidth * 0.5f; // Połowa szerokości toru
    if (minDist > half) {
        // Auto wyjechało poza tor – przesuń z powrotem na krawędź
        sf::Vector2f dir = pos - bestProj; // Wektor od osi do pozycji auta
        float len = std::sqrt(dir.x*dir.x + dir.y*dir.y);
        if (len > 0.f) dir /= len;         // Znormalizuj
        // Ustaw pozycję dokładnie na krawędzi toru (z małym marginesem)
        pos = bestProj + dir * (half - 5.f);
        return true; // Nastąpiło pchnięcie
    }
    return false; // Auto było na torze
}
