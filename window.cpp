/**
 * Copyright 2022 Tabelo, <https://github.com/tabeloapp>.
 *
 * This file is part of Tabelo-Qt, <https://github.com/tabeloapp/tabelo-qt>.
 *
 * Tabelo-Qt is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Tabelo-Qt is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Tabelo-Qt.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "window.h"

#include <QMenuBar>
#include <QScreen>
#include <QSettings>


Window::Window(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowIcon(QIcon(QStringLiteral(":/icons/apps/16/tabelo.svg")));

    createMenuBar();
    createStatusBar();

    loadSettings();
}

Window::~Window()
{
}


void Window::closeEvent(QCloseEvent *event)
{
    if (true) {
        // Store application properties
        saveSettings();

        event->accept();
    }
    else {
        event->ignore();
    }
}


void Window::loadSettings()
{
    QSettings settings;

    // Application property: Geometry
    const auto geometry = settings.value(QStringLiteral("Application/Geometry"), QByteArray()).toByteArray();
    if (!geometry.isEmpty()) {
        restoreGeometry(geometry);
    }
    else {
        // Default: Center window
        const auto availableGeometry = screen()->availableGeometry();
        resize(availableGeometry.width() * 2/3, availableGeometry.height() * 2/3);
        move((availableGeometry.width() - width()) / 2, (availableGeometry.height() - height()) / 2);
    }

    // Application property: State
    const auto state = settings.value(QStringLiteral("Application/State"), QByteArray()).toByteArray();
    if (!state.isEmpty()) {
        restoreState(state);
    }
}


void Window::saveSettings()
{
    QSettings settings;

    // Application property: Geometry
    const auto geometry = saveGeometry();
    settings.setValue(QStringLiteral("Application/Geometry"), geometry);

    // Application property: State
    const auto state = saveState();
    settings.setValue(QStringLiteral("Application/State"), state);
}


void Window::createMenuBar()
{
    // Menu: Application
    auto *menuApplication = menuBar()->addMenu(tr("Application"));
    menuApplication->setObjectName(QStringLiteral("menuApplication"));
}


void Window::createStatusBar()
{
    m_statusbar = statusBar();
    m_statusbar->showMessage(tr("Ready"), 3000);
}
