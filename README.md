# Jeźdźcy Pro+ – Gra wyścigowa 2D

Gra wyścigowa 2D napisana w C++ z SFML.

## Sterowanie
| Klawisz | Akcja |
|---------|-------|
| W | Gaz |
| S | Hamulec / cofanie |
| A | Skręt w lewo |
| D | Skręt w prawo |
| ESC | Powrót do menu |

## Jak uruchomić w Qt Creator

1. Sklonuj repozytorium
2. Otwórz Qt Creator
3. **Plik → Otwórz plik lub projekt**
4. Wybierz **CMakeLists.txt** (nie .pro)
5. Kliknij **Konfiguruj projekt**
6. Naciśnij **Ctrl+R**

> Jeśli CMake nie znajdzie SFML automatycznie, podaj ścieżkę:
> W Qt Creator: Projekty → CMake → dodaj zmienną:
> SFML_DIR = C:/SFML-2.5.1/lib/cmake/SFML

## Wymagania
- Qt Creator z CMake
- SFML 2.5 lub nowszy
- Kompilator C++17 (MinGW lub MSVC)

## Autorzy
- Adam Gryglik
- Szymon Fraska
