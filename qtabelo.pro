#
# Copyright 2022 naracanto <https://naracanto.github.io>.
#
# This file is part of QTabelo <https://github.com/cutelabs/qtabelo>.
#
# QTabelo is an open source table editor written in C++ using the
# Qt framework.
#
# QTabelo is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# QTabelo is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with QTabelo.  If not, see <https://www.gnu.org/licenses/>.
#

QT += core gui svg

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    about_dialog.cpp \
    colophon_dialog.cpp \
    colophon_pages.cpp \
    dialog_header_box.cpp \
    main.cpp \
    main_window.cpp \
    mdi_area.cpp \
    mdi_document.cpp \
    mdi_window.cpp \
    preferences_dialog.cpp \
    tabular_document.cpp

HEADERS += \
    about_dialog.h \
    colophon_dialog.h \
    colophon_pages.h \
    dialog_header_box.h \
    main_window.h \
    mdi_area.h \
    mdi_document.h \
    mdi_window.h \
    preferences_dialog.h \
    tabular_document.h

RESOURCES += \
    icons.qrc

CONFIG += lrelease

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target