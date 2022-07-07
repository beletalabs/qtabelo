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

#include "document_window.h"

#include <QAction>
#include <QDebug>
#include <QDir>
#include <QIcon>
#include <QMdiArea>
#include <QMenu>
#include <QMessageBox>
#include <QString>
#include <QUrl>
#include <QWidget>

#include "confirmation_dialog.h"
#include "document_widget.h"


DocumentWindow::DocumentWindow(QWidget *parent)
    : QMdiSubWindow(parent)
    , m_filenameSequenceNumber{0}
{
    setAttribute(Qt::WA_DeleteOnClose);

    setupActions();
}


void DocumentWindow::setupActions()
{
    QMenu *menu = systemMenu();
    if (!menu)
        return;

    m_actionClose = new QAction(tr("&Close"), this);
    m_actionClose->setObjectName(QStringLiteral("actionClose"));
    m_actionClose->setIcon(QIcon::fromTheme(QStringLiteral("window-close"), QIcon(QStringLiteral(":/icons/actions/16/window-close.svg"))));
    m_actionClose->setToolTip(tr("Close document"));
    connect(m_actionClose, &QAction::triggered, this, &DocumentWindow::close);

    m_actionCloseOther = new QAction(tr("Close Ot&her"), this);
    m_actionCloseOther->setObjectName(QStringLiteral("actionCloseOther"));
    m_actionCloseOther->setIcon(QIcon::fromTheme(QStringLiteral("window-close"), QIcon(QStringLiteral(":/icons/actions/16/window-close.svg"))));
    m_actionCloseOther->setToolTip(tr("Close other open documents"));
    connect(m_actionCloseOther, &QAction::triggered, this, &DocumentWindow::slotCloseOther);

    m_actionShowPath = new QAction(tr("Show &Path"), this);
    m_actionShowPath->setObjectName(QStringLiteral("actionShowPath"));
    m_actionShowPath->setCheckable(true);
    m_actionShowPath->setIcon(QIcon::fromTheme(QStringLiteral("show-path"), QIcon(QStringLiteral(":/icons/actions/16/show-path.svg"))));
    m_actionShowPath->setToolTip(tr("Show document path in the tab caption"));
    connect(m_actionShowPath, &QAction::toggled, this, &DocumentWindow::updateWindowTitle);

    m_actionCopyPath = new QAction(tr("Cop&y Path"), this);
    m_actionCopyPath->setObjectName(QStringLiteral("actionCopyPath"));
    m_actionCopyPath->setIcon(QIcon::fromTheme(QStringLiteral("edit-copy-path"), QIcon(QStringLiteral(":/icons/actions/16/edit-copy-path.svg"))));
    m_actionCopyPath->setToolTip(tr("Copy document path to clipboard"));
    connect(m_actionCopyPath, &QAction::triggered, this, &DocumentWindow::actionCopyPath);

    m_actionCopyFilename = new QAction(tr("Copy &Filename"), this);
    m_actionCopyFilename->setObjectName(QStringLiteral("actionCopyFilename"));
    m_actionCopyFilename->setIcon(QIcon::fromTheme(QStringLiteral("edit-copy-path"), QIcon(QStringLiteral(":/icons/actions/16/edit-copy-path.svg"))));
    m_actionCopyFilename->setToolTip(tr("Copy document filename to clipboard"));
    connect(m_actionCopyFilename, &QAction::triggered, this, &DocumentWindow::actionCopyFilename);

    m_actionRenameFilename = new QAction(tr("Re&name..."), this);
    m_actionRenameFilename->setObjectName(QStringLiteral("actionRename"));
    m_actionRenameFilename->setIcon(QIcon::fromTheme(QStringLiteral("edit-rename"), QIcon(QStringLiteral(":/icons/actions/16/edit-rename.svg"))));
    m_actionRenameFilename->setToolTip(tr("Rename file name of the document"));
    connect(m_actionRenameFilename, &QAction::triggered, this, &DocumentWindow::actionRenameFilename);

    menu->clear();
    menu->addAction(m_actionClose);
    menu->addAction(m_actionCloseOther);
    menu->addSeparator();
    menu->addAction(m_actionShowPath);
    menu->addAction(m_actionCopyPath);
    menu->addAction(m_actionCopyFilename);
    menu->addSeparator();
    menu->addAction(m_actionRenameFilename);
}


void DocumentWindow::enableActionCloseOther(const bool enabled)
{
    m_actionCloseOther->setEnabled(enabled);
}


//
// Property: filenameSequenceNumber
//

int DocumentWindow::filenameSequenceNumber() const
{
    return m_filenameSequenceNumber;
}


void DocumentWindow::setFilenameSequenceNumber(const int number)
{
    if (number != m_filenameSequenceNumber)
        m_filenameSequenceNumber = number;
}


void DocumentWindow::resetFilenameSequenceNumber()
{
    m_filenameSequenceNumber = 0;
}


int DocumentWindow::latestFilenameSequenceNumber(const QUrl &url) const
{
    int number = 0;

    if (mdiArea()) {
        const QList<QMdiSubWindow *> subWindows = mdiArea()->subWindowList();
        for (auto *subWindow : subWindows) {

            auto *document = qobject_cast<DocumentWidget *>(subWindow->widget());
            if (document->url().fileName() == url.fileName()) {

                auto *docWindow = qobject_cast<DocumentWindow *>(subWindow);
                if (docWindow->filenameSequenceNumber() > number)
                    number = docWindow->filenameSequenceNumber();
            }
        }
    }

    return number;
}


//
// Document window
//

QString DocumentWindow::windowCaption(const bool pathVisible) const
{
    if (!widget())
        return QString();

    QString caption = tr("Untitled");
    const QUrl &url = qobject_cast<DocumentWidget *>(widget())->url();

    // Name
    if (!url.isEmpty()) {

        if (pathVisible) {
            caption = url.toString(QUrl::PreferLocalFile);

            const QString &homePath = QDir::homePath();
            if (caption.startsWith(homePath))
                caption.replace(0, homePath.length(), QLatin1Char('~'));
        }
        else if (!url.fileName().isEmpty()) {
            caption = url.fileName();
        }
    }

    // Sequence number
    if ((!pathVisible || url.isEmpty()) && m_filenameSequenceNumber > 1)
        caption = tr("%1 (%2)").arg(caption).arg(m_filenameSequenceNumber);

    return caption;
}


void DocumentWindow::updateWindowTitle(const bool pathVisible)
{
    setWindowTitle(windowCaption(pathVisible));
}


void DocumentWindow::updateWindowIcon(const bool modified)
{
    QIcon icon;

    if (modified)
        icon = QIcon::fromTheme(QStringLiteral("document-save"), QIcon(QStringLiteral(":/icons/actions/16/document-save.svg")));

    setWindowIcon(icon);
}


//
// Document
//

void DocumentWindow::documentCountChanged(const int count)
{
    enableActionCloseOther(count >= 2);
}


void DocumentWindow::documentModifiedChanged(const bool modified)
{
    setWindowModified(modified);
    updateWindowIcon(modified);
}


void DocumentWindow::documentUrlChanged(const QUrl &url)
{
    resetFilenameSequenceNumber();
    setFilenameSequenceNumber(latestFilenameSequenceNumber(url) + 1);

    updateWindowTitle(m_actionShowPath->isChecked());

    m_actionShowPath->setEnabled(!url.isEmpty());
    m_actionCopyPath->setEnabled(!url.isEmpty());
    m_actionCopyFilename->setEnabled(!url.fileName().isEmpty());
    m_actionRenameFilename->setEnabled(!url.fileName().isEmpty());
}


//
// Action slots
//

void DocumentWindow::slotCloseOther()
{
    if (mdiArea() && mdiArea()->subWindowList().size() >= 2) {

        const QString &title = tr("Close all documents except this one");
        const QString &text = tr("This will close all open documents except this one.\n"
                                 "Are you sure you want to continue?");
        const QString &confirmationKey = QStringLiteral("ConfirmCloseOtherDocuments");

        if (ConfirmationDialog::warningContinueCancel(this, title, text, confirmationKey) != QMessageBox::Cancel)
            emit actionCloseOther(this);
    }
}
