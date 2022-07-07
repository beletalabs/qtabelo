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

#ifndef PROPERTIES_PAGES_H
#define PROPERTIES_PAGES_H

#include <QWidget>

#include <QUrl>


//
//
// Properties page: General
//

class PropertiesPageGeneral : public QWidget
{
    Q_OBJECT

public:
    explicit PropertiesPageGeneral(const QUrl &url, QWidget *parent = nullptr);

    QString title() const;
};


//
//
// Properties page: Permissions
//

class PropertiesPagePermissions : public QWidget
{
    Q_OBJECT

public:
    explicit PropertiesPagePermissions(const QUrl &url, QWidget *parent = nullptr);

    QString title() const;
};

#endif // PROPERTIES_PAGES_H
