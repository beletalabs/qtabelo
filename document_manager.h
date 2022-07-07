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

#ifndef DOCUMENT_MANAGER_H
#define DOCUMENT_MANAGER_H

#include <QMdiArea>

#include <QTabWidget>

class QMdiSubWindow;
class QUrl;
class QWidget;


class DocumentManager : public QMdiArea
{
    Q_OBJECT

    Q_PROPERTY(bool tabVisible MEMBER m_tabVisible READ isTabVisible WRITE setTabVisible RESET resetTabVisible NOTIFY tabVisibleChanged)
    Q_PROPERTY(QTabWidget::TabPosition tabPosition READ tabPosition WRITE setTabPosition RESET resetTabPosition NOTIFY tabPositionChanged)
    Q_PROPERTY(bool tabAutoHide MEMBER m_tabAutoHide READ isTabAutoHide WRITE setTabAutoHide RESET resetTabAutoHide NOTIFY tabAutoHideChanged)

public:
    explicit DocumentManager(QWidget *parent = nullptr);

    void saveSettings();

    bool isTabVisible() const;
    QTabWidget::TabPosition tabPosition() const;
    bool isTabAutoHide() const;
    bool hasTabBar() const;

    int subWindowCount() const;

    QMdiSubWindow *findSubWindow(const QUrl &url) const;

signals:
    void tabVisibleChanged(const bool visible);
    void tabPositionChanged(const QTabWidget::TabPosition position);
    void tabAutoHideChanged(const bool autoHide);

public slots:
    void setTabVisible(const bool visible);
    void resetTabVisible();
    void initTabVisible();

    void setTabPosition(const QTabWidget::TabPosition position);
    void resetTabPosition();
    void initTabPosition();

    void setTabAutoHide(const bool autoHide);
    void resetTabAutoHide();
    void initTabAutoHide();

    void closeSelectedSubWindow(QMdiSubWindow *subWindow);
    void closeOtherSubWindows(QMdiSubWindow *subWindow);

private:
    void loadSettings();

    void setTabBarVisible(const bool visible);
    void setTabBarAutoHide(const bool autoHide);

private:
    bool m_tabVisible;
    bool m_tabAutoHide;
};

#endif // DOCUMENT_MANAGER_H
