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

#ifndef RECENT_DOCUMENT_LIST_H
#define RECENT_DOCUMENT_LIST_H

#include <QObject>

class QAction;
class QUrl;


class RecentDocumentList : public QObject
{
    Q_OBJECT

    Q_PROPERTY(int count READ count)
    Q_PROPERTY(int maximum MEMBER m_maximum READ maximum WRITE setMaximum)

public:
    explicit RecentDocumentList(QObject *parent = nullptr);

    void addUrl(const QUrl &url);

    QList<QAction *> actions();
    QList<QUrl> urls() const;

    int count() const;
    int maximum() const;

signals:
    void changed();

public slots:
    void setMaximum(const int max);

    void clear();

private:
    QAction *findUrlAction(const QUrl &url) const;
    QString urlActionText(const QUrl &url) const;

private:
    QList<QAction *> m_documentList;

    int m_maximum;
};

#endif // RECENT_DOCUMENT_LIST_H
