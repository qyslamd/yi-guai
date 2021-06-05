#-------------------------------------------------
#
# Project created by zhouyohu 2020-09-08T10:30:29
#
#-------------------------------------------------

QT       -= core gui

TARGET = htmlcxx
TEMPLATE = lib
CONFIG += staticlib

TARGET  = htmlcxx
OBJECTS_DIR = .obj
MOC_DIR     = .moc

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += html/Extensions.cc \
    html/Node.cc \
    html/ParserDom.cc \
    html/ParserSax.cc \
    html/Uri.cc \
    html/utils.cc

HEADERS +=  html/ci_string.h \
    html/debug.h \
    html/Extensions.h \
    html/Node.h \
    html/ParserSax.h \
    html/tld.h \
    html/tree.h \
    html/Uri.h \
    html/utils.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
