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

#include "table_document.h"

#include <QSettings>
#include <QTabBar>
#include <QTabWidget>
#include <QVBoxLayout>


TableDocument::TableDocument(QWidget *parent)
    : QWidget(parent)
    , m_tabBox{new QTabWidget}
    , m_tabVisible{true}
{
    m_tabBox->setDocumentMode(true);
    m_tabBox->setMovable(true);
    m_tabBox->setTabsClosable(true);
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
}


void TableDocument::saveSettings()
{
    QSettings settings;

    // Sheet Tab Visible
    bool visible = isTabVisible();
    settings.setValue(QStringLiteral("Document/SheetTabVisible"), visible);
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
//
//

void TableDocument::setTabBarVisible(const bool visible)
{
    m_tabBox->tabBar()->setVisible(visible);
}


//
//
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
}
