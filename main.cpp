#include "Game.h"  // Główna klasa gry

// ============================================================
//  main.cpp – punkt wejścia programu
//  Tworzy obiekt Game i uruchamia główną pętlę
// ============================================================

int main()
{
    Game game; // Utwórz grę (inicjalizuje okno, zasoby, stany)
    game.run(); // Uruchom główną pętlę (blokuje do zamknięcia okna)
    return 0;   // Zakończ program z kodem 0 (sukces)
}
