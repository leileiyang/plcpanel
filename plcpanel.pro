#-------------------------------------------------
#
# Project created by QtCreator 2019-02-25T12:29:21
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = plcpanel
TEMPLATE = app


SOURCES += main.cpp\
        plcpanelwidget.cpp \
    statebutton.cpp \
    plcclient/plcclient.cc \
    dev/nmlgas.cpp \
    sampler/sampler.cpp \
    sampler/samplecontroller.cpp

HEADERS  += plcpanelwidget.h \
    statebutton.h \
    plcclient/plcclient.h \
    dev/gas.h \
    dev/nmlgas.h \
    sampler/sampler.h \
    sampler/samplecontroller.h

FORMS    += plcpanelwidget.ui

unix:!macx: LIBS += -L$$PWD/../../../../usr/local/lib/ -lplcmsg

INCLUDEPATH += $$PWD/../../../../usr/local/include
DEPENDPATH += $$PWD/../../../../usr/local/include

unix:!macx: LIBS += -L$$PWD/../../../../usr/local/lib/ -lrcs

INCLUDEPATH += $$PWD/../../../../usr/local/include
DEPENDPATH += $$PWD/../../../../usr/local/include
