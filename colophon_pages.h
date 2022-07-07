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

#ifndef COLOPHON_PAGES_H
#define COLOPHON_PAGES_H

#include <QWidget>


//
//
// Colophon page: About
//

class ColophonPageAbout : public QWidget
{
    Q_OBJECT

public:
    explicit ColophonPageAbout(QWidget *parent = nullptr);

    QString title() const;
};


//
//
// Colophon page: Authors
//

class ColophonPageAuthors : public QWidget
{
    Q_OBJECT

public:
    explicit ColophonPageAuthors(QWidget *parent = nullptr);

    QString title() const;
};


//
//
// Colophon page: Credits
//

class ColophonPageCredits : public QWidget
{
    Q_OBJECT

public:
    explicit ColophonPageCredits(QWidget *parent = nullptr);

    QString title() const;
};


//
//
// Colophon page: Environment
//

class ColophonPageEnvironment : public QWidget
{
    Q_OBJECT

public:
    explicit ColophonPageEnvironment(QWidget *parent = nullptr);

    QString title() const;
};


//
//
// Colophon page: License
//

class ColophonPageLicense : public QWidget
{
    Q_OBJECT

public:
    explicit ColophonPageLicense(QWidget *parent = nullptr);

    QString title() const;
};

#endif // COLOPHON_PAGES_H
