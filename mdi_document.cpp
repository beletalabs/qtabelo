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

#include <QApplication>
#include <QClipboard>
#include <QDebug>
#include <QWidget>


MdiDocument::MdiDocument(QWidget *parent)
    : TabularDocument{parent}
    , m_documentUrl{""}
{
    setAttribute(Qt::WA_DeleteOnClose);
}


QUrl MdiDocument::documentUrl() const
{
    return m_documentUrl;
}


void MdiDocument::setDocumentUrl(const QUrl &url)
{
    if (url != m_documentUrl) {
        m_documentUrl = url;

        emit documentUrlChanged(m_documentUrl);
    }
}


void MdiDocument::resetDocumentUrl()
{
    m_documentUrl = QUrl("");

    emit documentUrlChanged(m_documentUrl);
}


void MdiDocument::copyDocumentUrlToClipboard()
{
    if (!m_documentUrl.isEmpty())
        QApplication::clipboard()->setText(m_documentUrl.toDisplayString(QUrl::PreferLocalFile));
}
