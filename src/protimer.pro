#/********************************************************************************/
#/*                                                                              */
#/*    Copyright 2012 Alexander Vorobyev (Voral)                                 */
#/*    http://va-soft.ru/                                                        */
#/*                                                                              */
#/*    This file is part of protime.                                             */
#/*                                                                              */
#/*    Basetest is free software: you can redistribute it and/or modify          */
#/*    it under the terms of the GNU General Public License as published by      */
#/*    the Free Software Foundation, either version 3 of the License, or         */
#/*    (at your option) any later version.                                       */
#/*                                                                              */
#/*    Basetest is distributed in the hope that it will be useful,               */
#/*    but WITHOUT ANY WARRANTY; without even the implied warranty of            */
#/*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             */
#/*    GNU General Public License for more details.                              */
#/*                                                                              */
#/*    You should have received a copy of the GNU General Public License         */
#/*    along with protime.  If not, see <http://www.gnu.org/licenses/>.          */
#/*                                                                              */
#/********************************************************************************/

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = protimer

TEMPLATE = app

DESTDIR = ../bin
!macx {
        win32 {
                DESTDIR = ../bin32
        }
        linux-g++-32 {
                DESTDIR = ../bin32
        }
        linux-g++-64 {
                DESTDIR = ../bin64
        }
}


OBJECTS_DIR += ../.objsrv
MOC_DIR += ../.mocsrv
RCC_DIR += ../.rccsrv

SOURCES += main.cpp\
        mainwindow.cpp \
    project.cpp \
    edit.cpp \
    hotkeyedit.cpp

HEADERS  += mainwindow.h \
    project.h \
    edit.h \
    hotkeyedit.h

FORMS    += mainwindow.ui \
    project.ui \
    edit.ui \
    hotkeyedit.ui

RESOURCES += \
    protimer.qrc

TRANSLATIONS  = lang/protimer_ru.ts

QMAKE_CPPFLAGS *=$(shell dpkg-buildflags --get CPPFLAGS)
QMAKE_CFLAGS *= $(shell dpkg-buildflags --get CFLAGS)
QMAKE_CXXFLAGS *=$(shell dpkg-buildflags --get CXXFLAGS)
QMAKE_LDFLAGS *=$(shell dpkg-buildflags --get LDFLAGS)
