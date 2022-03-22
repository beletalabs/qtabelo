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


MdiDocument::MdiDocument(QWidget *parent)
    : TabularDocument{parent}
    , m_documentUrl{QString()}
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
