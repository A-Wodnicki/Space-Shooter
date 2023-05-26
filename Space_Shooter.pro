TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    Sources/background.cpp \
    Sources/game.cpp \
    Sources/main.cpp \
    Sources/playership.cpp \
    Sources/projectile.cpp

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
    Assets/Images/laserBlue01.png \
    README.md \
    Assets/Images/blue.png \
    Assets/Images/ship.png

HEADERS += \
    Headers/background.hpp \
    Headers/game.hpp \
    Headers/playership.hpp \
    Headers/projectile.hpp


