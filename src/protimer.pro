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
macx {
	DESTDIR = ../binmac
	TARGET = Protimer
}
win32 {
    DESTDIR = ../binwin
}
linux-g++-32 {
    DESTDIR = ../bin32
}
linux-g++-64 {
    DESTDIR = ../bin64
}


win32 {
        RC_FILE = icon.rc
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

OTHER_FILES += \
    icon.rc \
    android/res/layout/splash.xml \
    android/res/values-de/strings.xml \
    android/res/values-zh-rCN/strings.xml \
    android/res/values-et/strings.xml \
    android/res/values/strings.xml \
    android/res/values/libs.xml \
    android/res/values-ru/strings.xml \
    android/res/values-ro/strings.xml \
    android/res/values-it/strings.xml \
    android/res/values-id/strings.xml \
    android/res/drawable/icon.png \
    android/res/drawable/logo.png \
    android/res/values-fa/strings.xml \
    android/res/drawable-mdpi/icon.png \
    android/res/values-nb/strings.xml \
    android/res/values-es/strings.xml \
    android/res/values-nl/strings.xml \
    android/res/drawable-ldpi/icon.png \
    android/res/values-zh-rTW/strings.xml \
    android/res/values-pl/strings.xml \
    android/res/values-ms/strings.xml \
    android/res/drawable-hdpi/icon.png \
    android/res/values-el/strings.xml \
    android/res/values-pt-rBR/strings.xml \
    android/res/values-ja/strings.xml \
    android/res/values-fr/strings.xml \
    android/res/values-rs/strings.xml \
    android/AndroidManifest.xml \
    android/src/org/kde/necessitas/ministro/IMinistro.aidl \
    android/src/org/kde/necessitas/ministro/IMinistroCallback.aidl \
    android/src/org/kde/necessitas/origo/QtActivity.java \
    android/src/org/kde/necessitas/origo/QtApplication.java \
    android/version.xml
