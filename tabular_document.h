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

#ifndef TABULAR_DOCUMENT_H
#define TABULAR_DOCUMENT_H

#include <QWidget>


class TabularDocument : public QWidget
{
    Q_OBJECT

    Q_PROPERTY(bool modified MEMBER m_modified READ isModified WRITE setModified RESET resetModified NOTIFY modifiedChanged)

public:
    explicit TabularDocument(QWidget *parent = nullptr);

    bool isModified() const;
    void resetModified();

public slots:
    void setModified(bool modified = true);

signals:
    void modifiedChanged(const bool modified);

private:
    bool m_modified;
};

#endif // TABULAR_DOCUMENT_H
