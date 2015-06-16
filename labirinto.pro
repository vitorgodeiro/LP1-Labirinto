#-------------------------------------------------
#
# Project created by QtCreator 2015-05-20T10:30:04
#
#-------------------------------------------------

QT  += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = labirinto
TEMPLATE = app
#QMAKE_LFLAGS += -static

OTHER_FILES += \
    player.tga \
    iground.tga \
    wall.tga \
    oground.tga \
    item.tga \
    title.png

# Copy files post build
win32 {
    DESTDIR_WIN = $${DESTDIR}
    DESTDIR_WIN ~= s,/,\\,g
    PWD_WIN = $${PWD}
    PWD_WIN ~= s,/,\\,g
    for(FILE, OTHER_FILES){
        QMAKE_POST_LINK += $$quote(cmd /c copy /y $${PWD_WIN}\\$${FILE} $${DESTDIR_WIN}$$escape_expand(\\n\\t))
    }
}
unix {
    for(FILE, OTHER_FILES){
        QMAKE_POST_LINK += $$quote(cp $${PWD}/$${FILE} $${DESTDIR}$$escape_expand(\\n\\t))
    }
}


SOURCES += main.cpp\
    pathfinderai.cpp \
        mainwindow.cpp \
    debug.c \
    labirinto.cpp \
    player.cpp \
    game.cpp \
    rect.cpp \
    color.cpp \
    digitalclock.cpp \
    surface.cpp \
    sequence.cpp \
    queue.cpp \
    stack.cpp

HEADERS  += mainwindow.h \
    labirinto.h \
    debug.h \
    player.h \
    game.h \
    rect.h \
    color.h \
    digitalclock.h \
    surface.h \
    pathfinderai.h \
    sequence.h \
    queue.h \
    stack.h

FORMS    += mainwindow.ui

RESOURCES +=


