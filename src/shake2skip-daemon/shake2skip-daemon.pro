QT       += core dbus
QT       -= gui

TARGET = shake2skip-daemon
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

HEADERS += \
    cpp/shakedetector.h \
    cpp/mafwrendererproxy.h

SOURCES += cpp/main.cpp \
    cpp/shakedetector.cpp \
    cpp/mafwrendererproxy.cpp

VPATH += cpp

MOC_DIR = cpp/.mocs
OBJECTS_DIR = cpp/.objs

target.path = /opt/shake2skip/bin
INSTALLS += target
