#-------------------------------------------------
#
# Project created by QtCreator 2013-10-18T12:36:34
#
#-------------------------------------------------

QT          -=  core
QT          -=  network
QT          -=  gui

TARGET      = 	project
CONFIG      += 	console
CONFIG      -= 	app_bundle

TEMPLATE    =   app


SOURCES     +=  main.cpp \
                ../lib/serialib.cpp

HEADERS     +=  ../lib/serialib.h

