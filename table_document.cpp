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
    , m_tabVisible{true}
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

    // Sheet Tab Visible
    const bool visible = settings.value(QStringLiteral("Document/SheetTabVisible"), true).toBool();
    m_tabVisible = visible;

    // Sheet Tab Position
    const int value = settings.value(QStringLiteral("Document/SheetTabPosition"), QTabWidget::South).toInt();
    const QList<int> positions = {QTabWidget::North, QTabWidget::South};
    const QTabWidget::TabPosition position = positions.contains(value) ? static_cast<QTabWidget::TabPosition>(value) : QTabWidget::South;
    m_tabBox->setTabPosition(position);

    // Sheet Tab Auto Hide
    const bool autoHide = settings.value(QStringLiteral("Document/SheetTabAutoHide"), true).toBool();
    m_tabBox->setTabBarAutoHide(autoHide);
}


void TableDocument::saveSettings()
{
    QSettings settings;

    // Sheet Tab Visible
    bool visible = isTabVisible();
    settings.setValue(QStringLiteral("Document/SheetTabVisible"), visible);

    // Sheet Tab Position
    const QTabWidget::TabPosition position = tabPosition();
    settings.setValue(QStringLiteral("Document/SheetTabPosition"), position);

    // Sheet Tab Auto Hide
    bool autoHide = isTabAutoHide();
    settings.setValue(QStringLiteral("Document/SheetTabAutoHide"), autoHide);
}


//
// Property: tabVisible
//

bool TableDocument::isTabVisible() const
{
    return m_tabVisible;
}


void TableDocument::setTabVisible(const bool visible)
{
    if (visible != isTabVisible()) {
        m_tabVisible = visible;
        setTabBarVisible(visible);
        emit tabVisibleChanged(visible);
    }
}


void TableDocument::resetTabVisible()
{
    m_tabVisible = true;
    setTabBarVisible(m_tabVisible);
    emit tabVisibleChanged(m_tabVisible);
}


void TableDocument::initTabVisible()
{
    const bool visible = isTabVisible();
    setTabBarVisible(visible);
    emit tabVisibleChanged(visible);
}


//
// Property: tabPosition
//

QTabWidget::TabPosition TableDocument::tabPosition() const
{
    return m_tabBox->tabPosition();
}


void TableDocument::setTabPosition(const QTabWidget::TabPosition position)
{
    if (position != tabPosition()) {
        m_tabBox->setTabPosition(position);
        emit tabPositionChanged(position);
    }
}


void TableDocument::resetTabPosition()
{
    const QTabWidget::TabPosition position = QTabWidget::South;
    m_tabBox->setTabPosition(position);
    emit tabPositionChanged(position);
}


void TableDocument::initTabPosition()
{
    emit tabPositionChanged(tabPosition());
}


//
// Property: tabAutoHide
//

bool TableDocument::isTabAutoHide() const
{
    return m_tabBox->tabBarAutoHide();
}


void TableDocument::setTabAutoHide(const bool autoHide)
{
    if (autoHide != isTabAutoHide()) {
        m_tabBox->setTabBarAutoHide(autoHide);
        emit tabAutoHideChanged(autoHide);
    }
}


void TableDocument::resetTabAutoHide()
{
    const bool autoHide = true;
    m_tabBox->setTabBarAutoHide(autoHide);
    emit tabAutoHideChanged(autoHide);
}


void TableDocument::initTabAutoHide()
{
    const bool autoHide = isTabAutoHide();
    m_tabBox->setTabBarAutoHide(autoHide);
    emit tabAutoHideChanged(autoHide);
}


//
//
//

void TableDocument::setTabBarVisible(const bool visible)
{
    if (!(m_tabBox->count() <= 1 && isTabAutoHide()))
        m_tabBox->tabBar()->setVisible(visible);
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
