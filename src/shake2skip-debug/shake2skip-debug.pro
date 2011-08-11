QT       += core dbus
QT       -= gui

TARGET = shake2skip-debug
CONFIG   += console
CONFIG   -= app_bundle

CONFIG += mobility
MOBILITY += sensors

TEMPLATE = app

exists($$QMAKE_INCDIR_QT"/../qmsystem2/qmkeys.h"){
   DEFINES += Q_WS_MAEMO_6
}

INCLUDEPATH += ../shake2skip-lib/cpp
LIBS += ../lib/libshake2skip.a

HEADERS +=

SOURCES += cpp/main.cpp

VPATH += cpp

MOC_DIR = cpp/.mocs
OBJECTS_DIR = cpp/.objs

target.path = /opt/shake2skip/bin
INSTALLS += target
