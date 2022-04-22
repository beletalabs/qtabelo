/**
 * Copyright 2022 naracanto <https://naracanto.github.io>.
 *
 * This file is part of QTabelo <https://github.com/cutelabs/qtabelo>.
 *
 * QTabelo is an open source table editor written in C++ using the
 * Qt framework.
 *
 * QTabelo is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published
 * by the Free Software Foundation, either version 3 of the License,
 * or (at your option) any later version.
 *
 * QTabelo is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with QTabelo.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "document_manager.h"

#include <QList>
#include <QMdiSubWindow>
#include <QSettings>
#include <QTabBar>
#include <QUrl>
#include <QWidget>

#include "document_widget.h"


DocumentManager::DocumentManager(QWidget *parent)
    : QMdiArea(parent)
    , m_tabVisible{true}
{
    loadSettings();
}


void DocumentManager::loadSettings()
{
    QSettings settings;

    // Document Tab Visible
    bool visible = settings.value(QStringLiteral("DocumentManager/DocumentTabVisible"), true).toBool();
    m_tabVisible = visible;
    setTabBarVisible(visible);
}


void DocumentManager::saveSettings()
{
    QSettings settings;

    // Document Tab Visible
    bool visible = m_tabVisible;
    settings.setValue(QStringLiteral("DocumentManager/DocumentTabVisible"), visible);
}


//
// Property: tabsVisible
//

bool DocumentManager::isTabVisible() const
{
    return m_tabVisible;
}


void DocumentManager::setTabVisible(const bool visible)
{
    if (hasTabBar() && visible != m_tabVisible) {
        m_tabVisible = visible;
        setTabBarVisible(visible);
        emit tabVisibleChanged(visible);
    }
}


void DocumentManager::resetTabVisible()
{
    if (hasTabBar()) {
        const bool visible = true;
        m_tabVisible = visible;
        setTabBarVisible(visible);
        emit tabVisibleChanged(visible);
    }
}


void DocumentManager::initTabVisible()
{
    if (hasTabBar())
        emit tabVisibleChanged(m_tabVisible);
}


//
//
//

bool DocumentManager::hasTabBar() const
{
    return findChild<QTabBar *>() != nullptr;
}


bool DocumentManager::isTabBarVisible() const
{
    return hasTabBar() ? findChild<QTabBar *>()->isVisible() : false;
}


void DocumentManager::setTabBarVisible(const bool visible)
{
    if (hasTabBar())
        findChild<QTabBar *>()->setVisible(visible);
}


//
//
//

int DocumentManager::subWindowCount() const
{
    return subWindowList().size();
}


QMdiSubWindow *DocumentManager::findSubWindow(const QUrl &url) const
{
    if (url.isEmpty())
        return nullptr;

    const QList<QMdiSubWindow *> subWindows = subWindowList();
    for (auto *subWindow : subWindows) {

        auto *document = qobject_cast<DocumentWidget *>(subWindow->widget());
        if (document && document->url() == url)
            return subWindow;
    }

    return nullptr;
}


void DocumentManager::closeSelectedSubWindow(QMdiSubWindow *subWindow)
{
    if (!subWindow)
        return;

    subWindow->close();
}


void DocumentManager::closeOtherSubWindows(QMdiSubWindow *subWindow)
{
    QList<QMdiSubWindow *> subWindows = subWindowList();
    if (!subWindow || subWindows.isEmpty() || subWindows.indexOf(subWindow) < 0)
        return;

    // First remove the subwindow from the list that should not be closed
    subWindows.removeOne(subWindow);

    // Then close all other subwindows
    for (auto *subWindow : subWindows)
        closeSelectedSubWindow(subWindow);
}
