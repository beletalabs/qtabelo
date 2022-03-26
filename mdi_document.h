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


class MdiDocument : public TabularDocument
{
    Q_OBJECT

    Q_PROPERTY(QUrl documentUrl MEMBER m_documentUrl READ documentUrl WRITE setDocumentUrl NOTIFY documentUrlChanged)
    Q_PROPERTY(int filenameSequenceNumber MEMBER m_filenameSequenceNumber READ filenameSequenceNumber WRITE setFilenameSequenceNumber RESET resetFilenameSequenceNumber)
    Q_PROPERTY(bool pathVisibleInWindowTitle MEMBER m_pathVisibleInWindowTitle READ isPathVisibleInWindowTitle WRITE setPathVisibleInWindowTitle)

public:
    explicit MdiDocument(QWidget *parent = nullptr);

    QUrl documentUrl() const;
    int filenameSequenceNumber() const;
    bool isPathVisibleInWindowTitle() const;
    QString windowCaption(const bool pathVisible) const;

public slots:
    void setDocumentUrl(const QUrl &url);
    void setFilenameSequenceNumber(const int number);
    void resetFilenameSequenceNumber();
    void setPathVisibleInWindowTitle(const bool visible);

signals:
    void documentUrlChanged(const QUrl &url);

private:
    QUrl m_documentUrl;
    int m_filenameSequenceNumber;
    bool m_pathVisibleInWindowTitle;
};

#endif // MDI_DOCUMENT_H
