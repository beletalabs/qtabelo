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

#ifndef MDI_DOCUMENT_H
#define MDI_DOCUMENT_H

#include "tabular_document.h"

#include <QUrl>

class QWidget;


class MdiDocument : public TabularDocument
{
    Q_OBJECT

    Q_PROPERTY(QUrl documentUrl MEMBER m_documentUrl READ documentUrl WRITE setDocumentUrl RESET resetDocumentUrl NOTIFY documentUrlChanged)

public:
    explicit MdiDocument(QWidget *parent = nullptr);

    QUrl documentUrl() const;

signals:
    void documentUrlChanged(const QUrl &url);

public slots:
    void setDocumentUrl(const QUrl &url);
    void resetDocumentUrl();
    void copyDocumentUrlToClipboard();

private:
    QUrl m_documentUrl;
};

#endif // MDI_DOCUMENT_H
