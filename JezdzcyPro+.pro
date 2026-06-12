# ============================================================
#  JezdzcyPro+.pro
#  3rd party: SFML - https://www.sfml-dev.org (zlib/libpng license)
# ============================================================

TARGET   = JezdzcyPro+
TEMPLATE = app
CONFIG  += c++17
QT      -= core gui

SOURCES += \
    main.cpp          \
    Game.cpp          \
    Car.cpp           \
    Track.cpp         \
    Obstacle.cpp      \
    Coin.cpp          \
    HUD.cpp           \
    Menu.cpp          \
    RankingScreen.cpp \
    EndScreen.cpp     \
    Ranking.cpp

HEADERS += \
    Constants.h      \
    GameObject.h     \
    Car.h            \
    Track.h          \
    Obstacle.h       \
    Coin.h           \
    HUD.h            \
    Menu.h           \
    RankingScreen.h  \
    EndScreen.h      \
    Ranking.h        \
    Game.h

INCLUDEPATH += include

win32 {
    # Szukaj SFML automatycznie w popularnych lokalizacjach
    exists("C:/SFML-2.5.1/include/SFML/Graphics.hpp"):    SFML_PATH = "C:/SFML-2.5.1"
    exists("C:/SFML-2.6.1/include/SFML/Graphics.hpp"):    SFML_PATH = "C:/SFML-2.6.1"
    exists("C:/SFML-2.6.2/include/SFML/Graphics.hpp"):    SFML_PATH = "C:/SFML-2.6.2"
    exists("C:/SFML/include/SFML/Graphics.hpp"):           SFML_PATH = "C:/SFML"
    exists("C:/Libraries/SFML/include/SFML/Graphics.hpp"): SFML_PATH = "C:/Libraries/SFML"
    exists("$$PWD/SFML/include/SFML/Graphics.hpp"):        SFML_PATH = "$$PWD/SFML"

    isEmpty(SFML_PATH) {
        error("Nie znaleziono SFML! Pobierz SFML MinGW 64-bit ze strony sfml-dev.org i umiec w C:/SFML")
    }

    message("Uzywam SFML z: $$SFML_PATH")

    INCLUDEPATH += "$$SFML_PATH/include"
    LIBS        += -L"$$SFML_PATH/lib"

    CONFIG(debug, debug|release) {
        LIBS += -lsfml-graphics-d -lsfml-window-d -lsfml-system-d
    } else {
        LIBS += -lsfml-graphics -lsfml-window -lsfml-system
    }

    # Kopiuj DLL-ki automatycznie
    DLL_DIR = $$SFML_PATH/bin
    CONFIG(debug, debug|release) {
        QMAKE_POST_LINK = copy /Y \"$$shell_path($$DLL_DIR/sfml-graphics-d-2.dll)\" \"$$shell_path($$OUT_PWD/debug/)\" & copy /Y \"$$shell_path($$DLL_DIR/sfml-window-d-2.dll)\" \"$$shell_path($$OUT_PWD/debug/)\" & copy /Y \"$$shell_path($$DLL_DIR/sfml-system-d-2.dll)\" \"$$shell_path($$OUT_PWD/debug/)\"
    } else {
        QMAKE_POST_LINK = copy /Y \"$$shell_path($$DLL_DIR/sfml-graphics-2.dll)\" \"$$shell_path($$OUT_PWD/release/)\" & copy /Y \"$$shell_path($$DLL_DIR/sfml-window-2.dll)\" \"$$shell_path($$OUT_PWD/release/)\" & copy /Y \"$$shell_path($$DLL_DIR/sfml-system-2.dll)\" \"$$shell_path($$OUT_PWD/release/)\"
    }
}

unix:!macx {
    LIBS += -lsfml-graphics -lsfml-window -lsfml-system
}

macx {
    LIBS += -lsfml-graphics -lsfml-window -lsfml-system
}
