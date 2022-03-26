/**
 * Copyright 2022 naracanto <https://naracanto.github.io>.
 *
 * This file is part of Tabelo-Qt <https://github.com/cutelabs/tabelo-qt>.
 *
 * Tabelo-Qt is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Tabelo-Qt is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Tabelo-Qt.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "mdi_document.h"

#include <QDebug>
#include <QDir>


MdiDocument::MdiDocument(QWidget *parent)
    : TabularDocument{parent}
    , m_documentUrl{"0"}
    , m_filenameSequenceNumber{0}
{

}


QUrl MdiDocument::documentUrl() const
{
    return m_documentUrl;
}


void MdiDocument::setDocumentUrl(const QUrl &url)
{
    if (url != m_documentUrl) {
        m_documentUrl = url;

        emit documentUrlChanged(url);
    }
}


int MdiDocument::filenameSequenceNumber() const
{
    return m_filenameSequenceNumber;
}


void MdiDocument::setFilenameSequenceNumber(const int number)
{
    if (number != m_filenameSequenceNumber)
        m_filenameSequenceNumber = number;
}


void MdiDocument::resetFilenameSequenceNumber()
{
    m_filenameSequenceNumber = 0;
}


bool MdiDocument::isPathVisibleInWindowTitle() const
{
    return m_pathVisibleInWindowTitle;
}


void MdiDocument::setPathVisibleInWindowTitle(const bool visible)
{
    if (visible != m_pathVisibleInWindowTitle)
        m_pathVisibleInWindowTitle = visible;
}


QString MdiDocument::windowCaption(const bool pathVisible) const
{
    QString caption;

    // Name
    if (!m_documentUrl.isEmpty()) {

        if (pathVisible) {

            caption = m_documentUrl.toString(QUrl::PreferLocalFile);

            const QString homePath = QDir::homePath();
            if (caption.startsWith(homePath))
                caption.replace(0, homePath.length(), QLatin1Char('~'));
        }
        else {
            caption = m_documentUrl.isLocalFile() ? m_documentUrl.fileName() : tr("Untitled");
        }
    }
    else {
        caption = tr("Untitled");
    }

    // Sequence number
    if (m_filenameSequenceNumber > 1 && (!pathVisible || m_documentUrl.isEmpty()))
        caption = tr("%1 (%2)").arg(caption).arg(m_filenameSequenceNumber);

    return caption;
}
