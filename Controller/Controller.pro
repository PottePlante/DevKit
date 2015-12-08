#-------------------------------------------------
#
# Project created by QtCreator 2015-10-31T23:46:05
#
#-------------------------------------------------

QT       += core gui sql network xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Controller
TEMPLATE = app


SOURCES += WiFi.cpp \
    main.cpp\
    touchscreen.cpp \
    Controlpanel.cpp \
    PlanteDatabase.cpp \
    plantitems_ui.cpp \
    Planteliste.cpp

HEADERS  += WiFi.h \
    touchscreen.h \
    PlanteDatabase.h \
    Controlpanel.h \
    plantitems_ui.h \
    Planteliste.h

FORMS    += touchscreen.ui
