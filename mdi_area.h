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

#ifndef MDI_AREA_H
#define MDI_AREA_H

#include <QMdiArea>

class QMdiSubWindow;
class QUrl;
class QWidget;


class MdiArea : public QMdiArea
{
    Q_OBJECT

public:
    explicit MdiArea(QWidget *parent = nullptr);

    int subWindowCount() const;

    QMdiSubWindow *findSubWindow(const QUrl &url) const;

public slots:
    void closeSelectedSubWindow(QMdiSubWindow *subWindow);
    void closeOtherSubWindows(QMdiSubWindow *subWindow);
};

#endif // MDI_AREA_H
