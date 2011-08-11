QT       += core dbus
QT       -= gui

TARGET = shake2skip
TEMPLATE = lib
DESTDIR = ../lib

CONFIG += qt debug staticlib
CONFIG   -= app_bundle

CONFIG += mobility
MOBILITY += sensors

exists($$QMAKE_INCDIR_QT"/../qmsystem2/qmkeys.h"){
   DEFINES += Q_WS_MAEMO_6
}

HEADERS += \
    cpp/logutility.h \
    cpp/accelerometerfilter.h

SOURCES += \
    cpp/accelerometerfilter.cpp

VPATH += cpp

MOC_DIR = cpp/.mocs
OBJECTS_DIR = cpp/.objs

target.path = /opt/shake2skip/lib
INSTALLS += target
