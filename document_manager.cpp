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

#include <QDebug>
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
    , m_tabAutoHide{false}
{
    setTabBarVisible(true);
    QMdiArea::setTabPosition(QTabWidget::North);
    setTabBarAutoHide(false);

    loadSettings();
}


void DocumentManager::loadSettings()
{
    QSettings settings;

    // Document Tab Visible
    bool visible = settings.value(QStringLiteral("DocumentManager/DocumentTabVisible"), true).toBool();
    m_tabVisible = visible;
    setTabBarVisible(visible);

    // Document Tab Position
    int value = settings.value(QStringLiteral("DocumentManager/DocumentTabPosition"), QTabWidget::North).toInt();
    const QList<QTabWidget::TabPosition> positions = {QTabWidget::North, QTabWidget::South};
    const QTabWidget::TabPosition position = positions.contains(static_cast<QTabWidget::TabPosition>(value)) ? static_cast<QTabWidget::TabPosition>(value) : QTabWidget::North;
    QMdiArea::setTabPosition(position);

    // Document Tab Auto Hide
    bool hide = settings.value(QStringLiteral("DocumentManager/DocumentTabAutoHide"), false).toBool();
    m_tabAutoHide = hide;
    setTabBarAutoHide(hide);
}


void DocumentManager::saveSettings()
{
    QSettings settings;

    // Document Tab Visible
    bool visible = m_tabVisible;
    settings.setValue(QStringLiteral("DocumentManager/DocumentTabVisible"), visible);

    // Document Tab Position
    const QTabWidget::TabPosition position = tabPosition();
    settings.setValue(QStringLiteral("DocumentManager/DocumentTabPosition"), position);

    // Document Tab Auto Hide
    bool hide = isTabAutoHide();
    settings.setValue(QStringLiteral("DocumentManager/DocumentTabAutoHide"), hide);
}


//
// Property: tabVisible
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
// Property: tabPosition
//

QTabWidget::TabPosition DocumentManager::tabPosition() const
{
    return QMdiArea::tabPosition();
}


void DocumentManager::setTabPosition(const QTabWidget::TabPosition position)
{
    if (position != QMdiArea::tabPosition()) {
        QMdiArea::setTabPosition(position);
        emit tabPositionChanged(position);
    }
}


void DocumentManager::resetTabPosition()
{
    const QTabWidget::TabPosition position = QTabWidget::North;
    QMdiArea::setTabPosition(position);
    emit tabPositionChanged(position);
}


void DocumentManager::initTabPosition()
{
    emit tabPositionChanged(QMdiArea::tabPosition());
}


//
// Property: tabAutoHide
//

bool DocumentManager::isTabAutoHide() const
{
    return m_tabAutoHide;
}


void DocumentManager::setTabAutoHide(const bool hide)
{
    if (hasTabBar() && hide != isTabAutoHide()) {
        m_tabAutoHide = hide;
        setTabBarAutoHide(hide);
        emit tabAutoHideChanged(hide);
    }
}


void DocumentManager::resetTabAutoHide()
{
    if (hasTabBar()) {
        m_tabAutoHide = false;
        setTabBarAutoHide(false);
        emit tabAutoHideChanged(false);
    }
}


void DocumentManager::initTabAutoHide()
{
    if (hasTabBar()) {
        setTabBarAutoHide(isTabAutoHide());
        emit tabAutoHideChanged(isTabAutoHide());
    }
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
    if (hasTabBar() && !(subWindowCount() <= 1 && isTabAutoHide()))
        findChild<QTabBar *>()->setVisible(visible);
}


void DocumentManager::setTabBarAutoHide(const bool hide)
{
    if (hasTabBar())
        findChild<QTabBar *>()->setAutoHide(hide);
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
