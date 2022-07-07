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

#include "recent_document_list.h"

#include <QAction>
#include <QDebug>
#include <QDir>
#include <QList>
#include <QSettings>
#include <QUrl>


RecentDocumentList::RecentDocumentList(QObject *parent)
    : QObject(parent)
    , m_documentList{}
    , m_maximum{20}
    , m_restore{true}
{
    loadSettings();
}


void RecentDocumentList::loadSettings()
{
    QSettings settings;

    // Maximum Recent Documents
    const int value = settings.value(QStringLiteral("Application/MaximumRecentDocuments"), 20).toInt();
    m_maximum = value >= 0 && value <= 50 ? value : 20;

    // Restore Recent Documents
    m_restore = settings.value(QStringLiteral("Application/RestoreRecentDocuments"), true).toBool();

    // Recent Document List
    const int count = settings.beginReadArray(QStringLiteral("RecentDocuments"));
    for (int i = count; i >= 0; --i) {
        settings.setArrayIndex(i);
        addUrl(settings.value(QStringLiteral("Document"), QString()).toString());
    }
    settings.endArray();
}


void RecentDocumentList::saveSettings()
{
    QSettings settings;

    // Maximum Recent Documents
    const int value = m_maximum;
    settings.setValue(QStringLiteral("Application/MaximumRecentDocuments"), value);

    // Restore Recent Documents
    const bool restore = m_restore;
    settings.setValue(QStringLiteral("Application/RestoreRecentDocuments"), restore);

    // Recent Document List
    if (!m_restore)
        clear();
    settings.remove(QStringLiteral("RecentDocuments"));
    settings.beginWriteArray(QStringLiteral("RecentDocuments"));
    for (int i = 0; i < m_documentList.count(); ++i) {
        settings.setArrayIndex(i);
        settings.setValue(QStringLiteral("Document"), m_documentList.at(i)->data().toUrl().toString());
    }
    settings.endArray();
}


void RecentDocumentList::addUrl(const QUrl &url)
{
    if (!url.isValid())
        return;

    QAction *action = findUrlAction(url);
    if (!action) {
        // Create a new entry at the top
        action = new QAction(urlActionText(url), this);
        action->setData(url);
        connect(action, &QAction::triggered, this, &RecentDocumentList::slotUrlAction);
        m_documentList.prepend(action);
    }
    else {
        // Move existing entry to the top
        const int index = m_documentList.indexOf(action);
        m_documentList.move(index, 0);
    }

    while (m_documentList.size() > m_maximum)
        delete m_documentList.takeLast();

    emit listChanged();
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

    return urls;
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
    if (max != m_maximum) {
        m_maximum = max;

        shrinkDocumentList(max);
    }
}


bool RecentDocumentList::restore() const
{
    return m_restore;
}


void RecentDocumentList::setRestore(const bool restore)
{
    if (restore != m_restore)
        m_restore = restore;
}


void RecentDocumentList::clear()
{
    shrinkDocumentList(0);
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


void RecentDocumentList::shrinkDocumentList(const int max)
{
    if (m_documentList.count() > max) {

        while (m_documentList.count() > max)
            delete m_documentList.takeLast();

        emit listChanged();
    }
}


void RecentDocumentList::slotUrlAction()
{
    auto *action = qobject_cast<QAction *>(sender());
    if (action)
        emit documentSelected(action->data().toUrl());
}
