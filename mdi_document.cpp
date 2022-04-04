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

#include "mdi_document.h"

#include <QApplication>
#include <QClipboard>
#include <QDebug>
#include <QFile>
#include <QInputDialog>
#include <QMessageBox>
#include <QUrl>
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


void MdiDocument::renameDocumentFilename()
{
    if (m_documentUrl.isEmpty() || !m_documentUrl.isLocalFile())
        return;

    bool ok = false;
    const QString oldFileName = m_documentUrl.fileName();
    const QString newFileName = QInputDialog::getText(this, tr("Rename file"), tr("New file name"), QLineEdit::Normal, oldFileName, &ok);
    if (!ok || newFileName.isEmpty() || (newFileName == oldFileName))
        return;

    QUrl newUrl = m_documentUrl.adjusted(QUrl::RemoveFilename).toString() + newFileName;

    if (!QFile::rename(m_documentUrl.toLocalFile(), newUrl.toLocalFile())) {

        QString title = tr("File Already Exists");
        QString text = tr("A file with the name <em>%1</em> already exists!").arg(newUrl.fileName());

        QMessageBox::critical(this, title, text);
        return;
    }

    setDocumentUrl(newUrl);
}
