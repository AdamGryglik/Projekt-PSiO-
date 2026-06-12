#include "Ranking.h"
#include <fstream>    // Odczyt i zapis pliku
#include <algorithm>  // std::sort, std::min
#include <iostream>   // Komunikaty błędów

// ============================================================
//  Konstruktor – inicjalizuje puste rankingi i wczytuje z pliku
// ============================================================
Ranking::Ranking()
{
    // Zainicjalizuj puste wektory dla każdego toru
    for (int i = 0; i < 3; i++) {
        m_timeRanking[i] = {}; // Pusty ranking czasowy toru i
        m_coinRanking[i] = {}; // Pusty ranking monetowy toru i
    }
    load(); // Spróbuj wczytać istniejące dane
}

// ============================================================
//  load – wczytuje ranking z pliku tekstowego
//  Format: każda linia = "trackId timeSeconds coins"
// ============================================================
void Ranking::load()
{
    std::ifstream file(FILENAME); // Otwórz plik do odczytu
    if (!file.is_open()) return;  // Plik nie istnieje – zacznij od zera

    int   trackId;      // Identyfikator toru (0,1,2)
    float timeSec;      // Czas wyścigu w sekundach
    int   coins;        // Liczba monet

    // Czytaj dopóki są dane w pliku
    while (file >> trackId >> timeSec >> coins) {
        if (trackId < 0 || trackId > 2) continue; // Pomiń nieprawidłowe dane

        RankEntry entry;
        entry.timeSeconds = timeSec; // Zapamiętaj czas
        entry.coins       = coins;   // Zapamiętaj monety

        // Dodaj do obu rankingów dla tego toru
        m_timeRanking[trackId].push_back(entry);
        m_coinRanking[trackId].push_back(entry);
    }

    // Po wczytaniu posortuj i ogranicz do top 3
    for (int i = 0; i < 3; i++) {
        // Ranking czasowy: im mniejszy czas, tym lepszy wynik
        std::sort(m_timeRanking[i].begin(), m_timeRanking[i].end(),
            [](const RankEntry& a, const RankEntry& b) {
                return a.timeSeconds < b.timeSeconds; // Rosnąco po czasie
            });
        if (m_timeRanking[i].size() > RANKING_SIZE)
            m_timeRanking[i].resize(RANKING_SIZE); // Zostaw tylko top 3

        // Ranking monetowy: im więcej monet, tym lepszy wynik
        std::sort(m_coinRanking[i].begin(), m_coinRanking[i].end(),
            [](const RankEntry& a, const RankEntry& b) {
                return a.coins > b.coins; // Malejąco po monetach
            });
        if (m_coinRanking[i].size() > RANKING_SIZE)
            m_coinRanking[i].resize(RANKING_SIZE); // Zostaw tylko top 3
    }
}

// ============================================================
//  save – zapisuje wszystkie wyniki do pliku
// ============================================================
void Ranking::save() const
{
    std::ofstream file(FILENAME); // Otwórz plik do zapisu (nadpisz)
    if (!file.is_open()) {
        std::cerr << "[Ranking] Nie udalo sie zapisac do " << FILENAME << "\n";
        return;
    }

    // Zapisz każdy wpis z rankingu czasowego (ranking monetowy dzieli dane)
    for (int i = 0; i < 3; i++) {
        for (const auto& entry : m_timeRanking[i]) {
            // Format: trackId timeSeconds coins
            file << i << " " << entry.timeSeconds << " " << entry.coins << "\n";
        }
    }
}

// ============================================================
//  addResult – dodaje wynik i aktualizuje rankingi
//  Zwraca true jeśli wynik trafił do top 3 (czas lub monety)
// ============================================================
bool Ranking::addResult(int trackId, float timeSeconds, int coins)
{
    if (trackId < 0 || trackId > 2) return false; // Nieprawidłowy tor

    bool isTop = false; // Flaga czy wynik trafił do czołówki

    // --- Dodaj do rankingu czasowego ---
    RankEntry entry;
    entry.timeSeconds = timeSeconds;
    entry.coins       = coins;

    m_timeRanking[trackId].push_back(entry); // Dodaj nowy wpis
    // Posortuj rosnąco po czasie (najlepszy = najkrótszy czas)
    std::sort(m_timeRanking[trackId].begin(), m_timeRanking[trackId].end(),
        [](const RankEntry& a, const RankEntry& b) {
            return a.timeSeconds < b.timeSeconds;
        });
    if ((int)m_timeRanking[trackId].size() <= RANKING_SIZE) isTop = true; // W top 3
    if (m_timeRanking[trackId].size() > RANKING_SIZE)
        m_timeRanking[trackId].resize(RANKING_SIZE); // Ogranicz do 3

    // --- Dodaj do rankingu monetowego ---
    m_coinRanking[trackId].push_back(entry); // Dodaj nowy wpis
    // Posortuj malejąco po monetach (najlepszy = najwięcej monet)
    std::sort(m_coinRanking[trackId].begin(), m_coinRanking[trackId].end(),
        [](const RankEntry& a, const RankEntry& b) {
            return a.coins > b.coins;
        });
    if ((int)m_coinRanking[trackId].size() <= RANKING_SIZE) isTop = true; // W top 3
    if (m_coinRanking[trackId].size() > RANKING_SIZE)
        m_coinRanking[trackId].resize(RANKING_SIZE); // Ogranicz do 3

    save(); // Natychmiast zapisz po dodaniu wyniku
    return isTop;
}

// ============================================================
//  Gettery – zwracają kopie rankingów
// ============================================================
std::vector<RankEntry> Ranking::getTopTimes(int trackId) const
{
    if (trackId < 0 || trackId > 2) return {}; // Nieprawidłowy ID
    return m_timeRanking[trackId]; // Zwróć top 3 czasów
}

std::vector<RankEntry> Ranking::getTopCoins(int trackId) const
{
    if (trackId < 0 || trackId > 2) return {}; // Nieprawidłowy ID
    return m_coinRanking[trackId]; // Zwróć top 3 monetowych
}
