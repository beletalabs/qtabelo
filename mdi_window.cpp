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

#include "mdi_window.h"

#include <QAction>
#include <QDebug>
#include <QDir>
#include <QIcon>
#include <QMdiArea>
#include <QMenu>
#include <QString>
#include <QUrl>
#include <QWidget>

#include "mdi_area.h"
#include "mdi_document.h"


MdiWindow::MdiWindow(QWidget *parent)
    : QMdiSubWindow{parent}
    , m_filenameSequenceNumber{0}
{
    setAttribute(Qt::WA_DeleteOnClose);

    setupActions();
}


void MdiWindow::setupActions()
{
    QMenu *menu = systemMenu();
    if (!menu)
        return;

    m_actionClose = new QAction(tr("&Close"), this);
    m_actionClose->setObjectName(QStringLiteral("actionClose"));
    m_actionClose->setIcon(QIcon::fromTheme(QStringLiteral("window-close"), QIcon(QStringLiteral(":/icons/actions/16/window-close.svg"))));
    m_actionClose->setToolTip(tr("Close document"));
    connect(m_actionClose, &QAction::triggered, this, &MdiWindow::close);

    m_actionCloseOther = new QAction(tr("Close Ot&her"), this);
    m_actionCloseOther->setObjectName(QStringLiteral("actionCloseOther"));
    m_actionCloseOther->setIcon(QIcon::fromTheme(QStringLiteral("window-close"), QIcon(QStringLiteral(":/icons/actions/16/window-close.svg"))));
    m_actionCloseOther->setToolTip(tr("Close other open documents"));
    connect(this, &MdiWindow::actionCloseOtherIsEnabled, m_actionCloseOther, &QAction::setEnabled);
    connect(m_actionCloseOther, &QAction::triggered, this, &MdiWindow::onActionCloseOtherTriggered);

    m_actionShowPath = new QAction(tr("Show &Path"), this);
    m_actionShowPath->setObjectName(QStringLiteral("actionShowPath"));
    m_actionShowPath->setCheckable(true);
    m_actionShowPath->setChecked(false);
    m_actionShowPath->setToolTip(tr("Show document path in the tab caption"));
    connect(m_actionShowPath, &QAction::toggled, this, &MdiWindow::updateWindowTitle);

    m_actionCopyPath = new QAction(tr("Cop&y Path"), this);
    m_actionCopyPath->setObjectName(QStringLiteral("actionCopyPath"));
    m_actionCopyPath->setIcon(QIcon::fromTheme(QStringLiteral("edit-copy"), QIcon(QStringLiteral(":/icons/actions/16/edit-copy.svg"))));
    m_actionCopyPath->setToolTip(tr("Copy document path to clipboard"));
    connect(m_actionCopyPath, &QAction::triggered, this, &MdiWindow::actionCopyPathTriggered);

    menu->clear();
    menu->addAction(m_actionClose);
    menu->addAction(m_actionCloseOther);
    menu->addSeparator();
    menu->addAction(m_actionShowPath);
    menu->addAction(m_actionCopyPath);
}


int MdiWindow::filenameSequenceNumber() const
{
    return m_filenameSequenceNumber;
}


void MdiWindow::setFilenameSequenceNumber(const int number)
{
    if (number != m_filenameSequenceNumber)
        m_filenameSequenceNumber = number;
}


void MdiWindow::resetFilenameSequenceNumber()
{
    m_filenameSequenceNumber = 0;
}


int MdiWindow::latestFilenameSequenceNumber(const QUrl &url) const
{
    int number = 0;

    const QList<QMdiSubWindow *> subWindows = mdiArea()->subWindowList();
    for (auto *subWindow : subWindows) {

        auto *document = qobject_cast<MdiDocument *>(subWindow->widget());
        if (document->documentUrl().fileName() == url.fileName()) {

            auto *docWindow = qobject_cast<MdiWindow *>(subWindow);
            if (docWindow->filenameSequenceNumber() > number)
                number = docWindow->filenameSequenceNumber();
        }
    }

    return number;
}


bool MdiWindow::isPathVisibleInWindowTitle() const
{
    return m_actionShowPath->isChecked();
}


void MdiWindow::setPathVisibleInWindowTitle(const bool pathVisible)
{
    m_actionShowPath->setChecked(pathVisible);
}


QString MdiWindow::windowCaption(const bool pathVisible) const
{
    QString caption;
    const QUrl &url = qobject_cast<MdiDocument *>(widget())->documentUrl();

    // Name
    if (!url.isEmpty()) {

        if (pathVisible) {

            caption = url.toString(QUrl::PreferLocalFile);

            const QString homePath = QDir::homePath();
            if (caption.startsWith(homePath))
                caption.replace(0, homePath.length(), QLatin1Char('~'));
        }
        else {
            caption = url.isLocalFile() ? url.fileName() : tr("Untitled");
        }
    }
    else {
        caption = tr("Untitled");
    }

    // Sequence number
    if (m_filenameSequenceNumber > 1 && (!pathVisible || url.isEmpty()))
        caption = tr("%1 (%2)").arg(caption).arg(m_filenameSequenceNumber);

    return caption;
}


void MdiWindow::updateWindowTitle(const bool pathVisible)
{
    setWindowTitle(windowCaption(pathVisible));
}


void MdiWindow::updateWindowIcon(const bool modified)
{
    QIcon icon;

    if (modified)
        icon = QIcon::fromTheme(QStringLiteral("document-save"), QIcon(QStringLiteral(":/icons/actions/16/document-save.svg")));

    setWindowIcon(icon);
}


void MdiWindow::documentUrlChanged(const QUrl &url)
{
    resetFilenameSequenceNumber();
    setFilenameSequenceNumber(latestFilenameSequenceNumber(url) + 1);

    bool checked = false;
    if (!url.isEmpty()) {
        checked = m_actionShowPath->isChecked() || m_actionShowPath->data().toBool();
        m_actionShowPath->data().clear();
    }
    else {
        m_actionShowPath->setData(m_actionShowPath->isChecked());
    }

    updateWindowTitle(checked);
    m_actionShowPath->setChecked(checked);

    m_actionShowPath->setEnabled(!url.isEmpty());
    m_actionCopyPath->setEnabled(!url.isEmpty());
}

void MdiWindow::onActionCloseOtherTriggered()
{
    emit actionCloseOtherTriggered(this);
}
