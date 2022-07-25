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

#ifndef DOCUMENT_WIDGET_H
#define DOCUMENT_WIDGET_H

#include "table_document.h"

#include <QUrl>

class QCloseEvent;
class QWidget;


class DocumentWidget : public TableDocument
{
    Q_OBJECT

    Q_PROPERTY(bool modified MEMBER m_modified READ isModified WRITE setModified RESET resetModified NOTIFY modifiedChanged)
    Q_PROPERTY(QUrl url MEMBER m_url READ url WRITE setUrl RESET resetUrl NOTIFY urlChanged)

public:
    explicit DocumentWidget(QWidget *parent = nullptr);

    bool isModified() const;
    void initModified();

    QUrl url() const;
    void initUrl();

signals:
    void modifiedChanged(const bool modified);
    void urlChanged(const QUrl &url);

public slots:
    void setModified(const bool modified);
    void resetModified();

    void setUrl(const QUrl &url);
    void resetUrl();

    void documentCountChanged(const int count);

    void copyPathToClipboard();
    void copyFilenameToClipboard();
    void renameFilename();

protected:
    void closeEvent(QCloseEvent *event) override;

private:
    bool m_modified;
    QUrl m_url;
};

#endif // DOCUMENT_WIDGET_H
