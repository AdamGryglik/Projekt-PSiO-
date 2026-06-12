#pragma once

#include <string>   // std::string
#include <vector>   // std::vector
#include <array>    // std::array
#include "Constants.h" // Stałe (RANKING_SIZE, liczba torów)

// ============================================================
//  Ranking.h – system zapisu i odczytu rankingów
//  Przechowuje top 3 czasów i monet dla każdego z 3 torów
//  Dane zapisywane do pliku tekstowego ranking.txt
// ============================================================

// Pojedynczy wpis rankingowy (czas lub monety)
struct RankEntry {
    float timeSeconds; // Czas okrążenia / wyścigu w sekundach
    int   coins;       // Liczba zebranych monet
};

class Ranking {
public:
    // Konstruktor – wczytuje ranking z pliku (jeśli istnieje)
    Ranking();

    // Zapisuje ranking do pliku (wywołaj po każdym wyścigu)
    void save() const;

    // Zwraca top 3 wpisów czasowych dla danego toru
    std::vector<RankEntry> getTopTimes(int trackId) const;

    // Zwraca top 3 wpisów monetowych dla danego toru
    std::vector<RankEntry> getTopCoins(int trackId) const;

    // Dodaje nowy wynik dla danego toru i automatycznie sortuje
    // Zwraca true jeśli wynik trafił do top 3
    bool addResult(int trackId, float timeSeconds, int coins);

private:
    // Macierz: [trackId][rank] = wpis
    std::array<std::vector<RankEntry>, 3> m_timeRanking; // Top 3 czasów dla 3 torów
    std::array<std::vector<RankEntry>, 3> m_coinRanking; // Top 3 monet dla 3 torów

    // Ścieżka pliku danych
    static constexpr const char* FILENAME = "ranking.txt";

    // Wczytuje dane z pliku
    void load();
};
