TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    Sources/background.cpp \
    Sources/game.cpp \
    Sources/main.cpp

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
    README.md \
    Assets/Images/blue.png \
    Assets/Images/ship.png

HEADERS += \
    Headers/background.h \
    Headers/game.h


