#pragma once  // Zabezpieczenie przed wielokrotnym dołączeniem headera

// ============================================================
//  Constants.h – wszystkie stałe gry w jednym miejscu
//  Zmiana wartości tutaj wpływa na całą grę
// ============================================================

// --- Okno ---
constexpr unsigned int WINDOW_WIDTH  = 900;   // Szerokość okna w pikselach
constexpr unsigned int WINDOW_HEIGHT = 600;   // Wysokość okna w pikselach
constexpr unsigned int FRAMERATE_LIMIT = 60;  // Limit klatek na sekundę

// --- Tor / trasa ---
constexpr int TOTAL_LAPS = 3;   // Liczba okrążeń do ukończenia wyścigu

// --- Fizyka auta ---
constexpr float CAR_MAX_SPEED        = 556.f; // Maksymalna prędkość do przodu [px/s] = 200 km/h
constexpr float CAR_MAX_REVERSE      = 83.f;  // Maksymalna prędkość do tyłu [px/s] = 30 km/h
constexpr float CAR_ACCELERATION     = 180.f; // Przyspieszenie przy gazie [px/s²]
constexpr float CAR_BRAKE_DECEL      = 350.f; // Opóźnienie przy hamowaniu [px/s²]
constexpr float CAR_FRICTION         = 120.f; // Tarcie – pasywne zwalnianie [px/s²]
constexpr float CAR_TURN_SPEED       = 150.f; // Prędkość skrętu [stopni/s]
constexpr float CAR_COLLISION_BOUNCE = 0.5f;  // Współczynnik odbicia przy kolizji
constexpr float CAR_COLLISION_SLOW   = 0.4f;  // Redukcja prędkości przy uderzeniu

// --- Rozmiar auta ---
constexpr float CAR_WIDTH  = 20.f; // Szerokość prostokąta auta [px]
constexpr float CAR_HEIGHT = 36.f; // Długość prostokąta auta [px]

// --- Przeszkody ---
constexpr float OBSTACLE_RADIUS = 14.f; // Promień beczki / pachołka [px]

// --- Monety ---
constexpr float  COIN_RADIUS = 10.f;  // Promień monety [px]
constexpr int    COIN_POINTS = 50;    // Punkty za zebraną monetę
constexpr float  COIN_COLLECT_DIST = COIN_RADIUS + CAR_WIDTH * 0.5f; // Dystans zebrania

// --- Ranking ---
constexpr int RANKING_SIZE = 3; // Ile najlepszych wyników trzymamy dla każdego toru

// --- Tory (ID) ---
constexpr int TRACK_GRASS  = 0; // Indeks toru trawiastego
constexpr int TRACK_SAND   = 1; // Indeks toru piaszczystego
constexpr int TRACK_CITY   = 2; // Indeks toru miejskiego

// --- Kolory aut ---
// Czerwone auto
constexpr unsigned CAR1_R = 220, CAR1_G = 40,  CAR1_B = 40;
// Niebieskie auto
constexpr unsigned CAR2_R = 40,  CAR2_G = 80,  CAR2_B = 220;
