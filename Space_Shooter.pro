TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    game.cpp \
    main.cpp \
    background.cpp
INCLUDEPATH += "D:/SFML-2.5.1/include"
LIBS += -L"D:/SFML-2.5.1/lib"
CONFIG(debug, debug|release){
    LIBS += -lsfml-audio-d -lsfml-graphics-d -lsfml-network-d -lsfml-system-d -lsfml-window-d
} else {
    LIBS += -lsfml-audio -lsfml-graphics -lsfml-network -lsfml-system -lsfml-window
}

DISTFILES += \
    README.md \
    assets/images/blue.png \
    assets/images/ship.png

HEADERS += \
    background.h \
    game.h


