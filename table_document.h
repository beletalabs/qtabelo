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

class QTabWidget;


class TableDocument : public QWidget
{
    Q_OBJECT

public:
    explicit TableDocument(QWidget *parent = nullptr);

protected:
    void slotAddTab(const int count);

private slots:
    void slotCloseTab(const int index);

private:
    QTabWidget *m_tabBox;
};

#endif // TABLE_DOCUMENT_H
