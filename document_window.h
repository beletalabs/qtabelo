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

#ifndef DOCUMENT_WINDOW_H
#define DOCUMENT_WINDOW_H

#include <QMdiSubWindow>

class QAction;
class QString;
class QUrl;
class QWidget;


class DocumentWindow : public QMdiSubWindow
{
    Q_OBJECT

    Q_PROPERTY(int filenameSequenceNumber MEMBER m_filenameSequenceNumber READ filenameSequenceNumber WRITE setFilenameSequenceNumber RESET resetFilenameSequenceNumber)

public:
    explicit DocumentWindow(QWidget *parent = nullptr);

    int filenameSequenceNumber() const;
    void initFilenameSequenceNumber();

    QString windowCaption(const bool pathVisible) const;

signals:
    void actionCloseOther(DocumentWindow *window);
    void actionCopyPath();
    void actionCopyFilename();
    void actionRenameFilename();

public slots:
    void setFilenameSequenceNumber(const int number);
    void resetFilenameSequenceNumber();

    void documentCountChanged(const int count);
    void documentModifiedChanged(const bool modified);
    void documentUrlChanged(const QUrl &url);

private:
    void setupActions();
    void enableActionCloseOther(const bool enabled);

    int latestFilenameSequenceNumber(const QUrl &url) const;

    void updateWindowTitle(const bool pathVisible);
    void updateWindowIcon(const bool modified);

private slots:
    void slotCloseOther();

private:
    int m_filenameSequenceNumber;

    QAction *m_actionClose;
    QAction *m_actionCloseOther;
    QAction *m_actionShowPath;
    QAction *m_actionCopyPath;
    QAction *m_actionCopyFilename;
    QAction *m_actionRenameFilename;
};

#endif // DOCUMENT_WINDOW_H
