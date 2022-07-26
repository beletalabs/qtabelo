/**
 * Copyright 2022 naracanto <https://naracanto.github.io>.
 *
 * This file is part of QTabelo <https://github.com/beletalabs/qtabelo>.
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

#include "table_document.h"

#include <QSettings>
#include <QTabBar>
#include <QVBoxLayout>


TableDocument::TableDocument(QWidget *parent)
    : QWidget(parent)
    , m_tabBox{new QTabWidget}
    , m_tabBarVisible{true}
{
    m_tabBox->setDocumentMode(true);
    m_tabBox->setMovable(true);
//    m_tabBox->setTabsClosable(true);
    m_tabBox->setTabPosition(QTabWidget::South);
    m_tabBox->setTabBarAutoHide(true);
    connect(m_tabBox, &QTabWidget::tabCloseRequested, this, &TableDocument::slotCloseTab);

    loadSettings();

    // Main layout
    auto *mainLayout = new QVBoxLayout();
    mainLayout->addWidget(m_tabBox);
    setLayout(mainLayout);
}


void TableDocument::loadSettings()
{
    QSettings settings;

    // Sheet Tab Bar Visible
    const bool visible = settings.value(QStringLiteral("Document/SheetTabBarVisible"), true).toBool();
    m_tabBarVisible = visible;

    // Sheet Tab Bar Position
    const int value = settings.value(QStringLiteral("Document/SheetTabBarPosition"), QTabWidget::South).toInt();
    const QList<int> values = {QTabWidget::North, QTabWidget::South};
    const QTabWidget::TabPosition position = values.contains(value) ? static_cast<QTabWidget::TabPosition>(value) : QTabWidget::South;
    m_tabBox->setTabPosition(position);

    // Sheet Tab Bar Auto Hide
    const bool autoHide = settings.value(QStringLiteral("Document/SheetTabBarAutoHide"), true).toBool();
    m_tabBox->setTabBarAutoHide(autoHide);
}


void TableDocument::saveSettings()
{
    QSettings settings;

    // Sheet Tab Bar Visible
    const bool visible = isTabBarVisible();
    settings.setValue(QStringLiteral("Document/SheetTabBarVisible"), visible);

    // Sheet Tab Bar Position
    const QTabWidget::TabPosition position = tabBarPosition();
    settings.setValue(QStringLiteral("Document/SheetTabBarPosition"), position);

    // Sheet Tab Bar Auto Hide
    const bool autoHide = isTabBarAutoHide();
    settings.setValue(QStringLiteral("Document/SheetTabBarAutoHide"), autoHide);
}


//
// Property: tabBarVisible
//

bool TableDocument::isTabBarVisible() const
{
    return m_tabBarVisible;
}


void TableDocument::setTabBarVisible(const bool visible)
{
    if (visible != m_tabBarVisible) {
        m_tabBarVisible = visible;
        _setTabBarVisible(m_tabBarVisible);
        emit tabBarVisibleChanged(m_tabBarVisible);
    }
}


void TableDocument::resetTabBarVisible()
{
    m_tabBarVisible = true;
    _setTabBarVisible(m_tabBarVisible);
    emit tabBarVisibleChanged(m_tabBarVisible);
}


void TableDocument::initTabBarVisible()
{
    _setTabBarVisible(m_tabBarVisible);
    emit tabBarVisibleChanged(m_tabBarVisible);
}


void TableDocument::_setTabBarVisible(const bool visible)
{
    if (!(m_tabBox->count() <= 1 && m_tabBox->tabBarAutoHide()))
        m_tabBox->tabBar()->setVisible(visible);
}


//
// Property: tabBarPosition
//

QTabWidget::TabPosition TableDocument::tabBarPosition() const
{
    return m_tabBox->tabPosition();
}


void TableDocument::setTabBarPosition(const QTabWidget::TabPosition position)
{
    if (position != tabBarPosition()) {
        m_tabBox->setTabPosition(position);
        emit tabBarPositionChanged(position);
    }
}


void TableDocument::resetTabBarPosition()
{
    m_tabBox->setTabPosition(QTabWidget::South);
    emit tabBarPositionChanged(QTabWidget::South);
}


void TableDocument::initTabBarPosition()
{
    emit tabBarPositionChanged(tabBarPosition());
}


//
// Property: tabBarAutoHide
//

bool TableDocument::isTabBarAutoHide() const
{
    return m_tabBox->tabBarAutoHide();
}


void TableDocument::setTabBarAutoHide(const bool autoHide)
{
    if (autoHide != isTabBarAutoHide()) {
        m_tabBox->setTabBarAutoHide(autoHide);
        emit tabBarAutoHideChanged(autoHide);
    }
}


void TableDocument::resetTabBarAutoHide()
{
    m_tabBox->setTabBarAutoHide(true);
    emit tabBarAutoHideChanged(true);
}


void TableDocument::initTabBarAutoHide()
{
    emit tabBarAutoHideChanged(isTabBarAutoHide());
}


//
// Slots
//

void TableDocument::slotAddTab(const int count)
{
    if (!m_tabBox->count()) {

        for (int i = 1; i <= count; ++i) {
            auto *widget = new QWidget;
            widget->setAttribute(Qt::WA_DeleteOnClose);
            m_tabBox->addTab(widget, tr("Sheet %1").arg(i));
        }
    }

    if (m_tabBox->count() > 1)
        m_tabBox->setTabsClosable(true);
}


void TableDocument::slotCloseTab(const int index)
{
    if (m_tabBox->count() > 1) {

        auto *widget = m_tabBox->widget(index);
        if (widget) {
            widget->close();
            m_tabBox->removeTab(index);
        }
    }

    if (m_tabBox->count() <= 1)
        m_tabBox->setTabsClosable(false);
}
