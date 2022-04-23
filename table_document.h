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

#ifndef TABLE_DOCUMENT_H
#define TABLE_DOCUMENT_H

#include <QWidget>

#include <QTabWidget>


class TableDocument : public QWidget
{
    Q_OBJECT

    Q_PROPERTY(bool tabVisible MEMBER m_tabVisible READ isTabVisible WRITE setTabVisible RESET resetTabVisible NOTIFY tabVisibleChanged)
    Q_PROPERTY(QTabWidget::TabPosition tabPosition READ tabPosition WRITE setTabPosition RESET resetTabPosition NOTIFY tabPositionChanged)

public:
    explicit TableDocument(QWidget *parent = nullptr);

    void saveSettings();

    bool isTabVisible() const;
    QTabWidget::TabPosition tabPosition() const;

signals:
    void tabVisibleChanged(const bool visible);
    void tabPositionChanged(const QTabWidget::TabPosition position);

public slots:
    void setTabVisible(const bool visible);
    void resetTabVisible();
    void initTabVisible();

    void setTabPosition(const QTabWidget::TabPosition position);
    void resetTabPosition();
    void initTabPosition();

protected:
    void slotAddTab(const int count);

private:
    void loadSettings();

    void setTabBarVisible(const bool visible);

private slots:
    void slotCloseTab(const int index);

private:
    QTabWidget *m_tabBox;

    bool m_tabVisible;
};

#endif // TABLE_DOCUMENT_H
