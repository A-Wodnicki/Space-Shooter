TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    Sources/background.cpp \
    Sources/bonus.cpp \
    Sources/enemy.cpp \
    Sources/explosion.cpp \
    Sources/game.cpp \
    Sources/main.cpp \
    Sources/player.cpp \
    Sources/projectile.cpp \
    Sources/ship.cpp

INCLUDEPATH += \
    "D:/SFML-2.5.1/include" \
    "Sources/" \
    "Headers/"

LIBS += -L"D:/SFML-2.5.1/lib"
CONFIG(debug, debug|release){
    LIBS += -lsfml-audio-d -lsfml-graphics-d -lsfml-network-d -lsfml-system-d -lsfml-window-d
} else {
    LIBS += -lsfml-audio -lsfml-graphics -lsfml-network -lsfml-system -lsfml-window
}

DISTFILES += \
    Assets/Images/bonus.png \
    Assets/Images/enemy1.png \
    Assets/Images/enemy2.png \
    Assets/Images/enemy3.png \
    Assets/Images/enemy4.png \
    Assets/Images/enemy5.png \
    Assets/Images/enemy6.png \
    Assets/Images/explosion.png \
    Assets/Images/laserBlue01.png \
    Assets/Images/laserRed01.png \
    Assets/Images/player.png \
    Assets/fonts/kenvector_future.ttf \
    README.md \
    Assets/Images/blue.png \
    Assets/Images/ship.png

HEADERS += \
    Headers/background.hpp \
    Headers/bonus.hpp \
    Headers/enemy.hpp \
    Headers/explosion.hpp \
    Headers/game.hpp \
    Headers/player.hpp \
    Headers/projectile.hpp \
    Headers/ship.hpp


