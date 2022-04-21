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

#include "recent_document_list.h"

#include <QAction>
#include <QDebug>
#include <QDir>
#include <QList>
#include <QUrl>


RecentDocumentList::RecentDocumentList(QObject *parent)
    : QObject(parent)
    , m_documentList{}
    , m_maximum{10}
{

}


void RecentDocumentList::addUrl(const QUrl &url)
{
    if (url.isEmpty() || !url.isValid())
        return;

    QAction *action = findUrlAction(url);
    if (!action) {
        // Create a new entry at the top
        action = new QAction(urlActionText(url), this);
        action->setData(url);
        m_documentList.prepend(action);
    }
    else {
        // Move existing entry to the top
        const int index = m_documentList.indexOf(action);
        m_documentList.move(index, 0);
    }

    while (m_documentList.size() > m_maximum)
        delete m_documentList.takeLast();

    emit changed();
}


QList<QAction *> RecentDocumentList::actions()
{
    return m_documentList;
}


QList<QUrl> RecentDocumentList::urls() const
{
    QList<QUrl> urls;

    for (auto *document : m_documentList)
        urls.append(document->data().toUrl());

    return QList<QUrl>();
}


int RecentDocumentList::count() const
{
    return m_documentList.count();
}


int RecentDocumentList::maximum() const
{
    return m_maximum;
}


void RecentDocumentList::setMaximum(const int max)
{
    if (max != m_maximum)
        m_maximum = max;
}


void RecentDocumentList::clear()
{
    while (m_documentList.count() > 0)
        delete m_documentList.takeLast();

    emit changed();
}


QAction *RecentDocumentList::findUrlAction(const QUrl &url) const
{
    for (auto *action : m_documentList)
        if (action->data().toUrl() == url)
            return action;

    return nullptr;
}


QString RecentDocumentList::urlActionText(const QUrl &url) const
{
    QString text = url.toDisplayString(QUrl::PreferLocalFile);

    const QString &homePath = QDir::homePath();
    if (text.startsWith(homePath))
        text.replace(0, homePath.length(), QLatin1Char('~'));

    text = tr("[%1]").arg(text);

    if (!url.fileName().isEmpty())
        text = tr("%1 %2").arg(url.fileName(), text);

    return text;
}
