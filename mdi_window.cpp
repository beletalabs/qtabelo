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

#include <QDebug>
#include <QIcon>
#include <QMenu>


MdiWindow::MdiWindow(QWidget *parent)
    : QMdiSubWindow{parent}
{
    setupActions();
}


void MdiWindow::setupActions()
{
    QMenu *menu = systemMenu();
    if (!menu)
        return;

    auto *actionClose = new QAction(tr("&Close Document"), this);
    actionClose->setObjectName(QStringLiteral("actionClose"));
    actionClose->setIcon(QIcon::fromTheme(QStringLiteral("window-close"), QIcon(QStringLiteral(":/icons/actions/16/window-close.svg"))));
    actionClose->setToolTip(tr("Close document"));
    connect(actionClose, &QAction::triggered, this, &MdiWindow::close);

    auto *actionCloseOther = new QAction(tr("Close Ot&her Documents"), this);
    actionCloseOther->setObjectName(QStringLiteral("actionCloseOther"));
    actionCloseOther->setIcon(QIcon::fromTheme(QStringLiteral("window-close"), QIcon(QStringLiteral(":/icons/actions/16/window-close.svg"))));
    actionCloseOther->setToolTip(tr("Close all other documents"));
    connect(this, &MdiWindow::enableActionCloseOther, actionCloseOther, &QAction::setEnabled);
    connect(actionCloseOther, &QAction::triggered, this, &MdiWindow::onActionCloseOtherTriggered);

    auto actionFullPath = new QAction(tr("Show Document &Path"), this);
    actionFullPath->setObjectName(QStringLiteral("actionFullPath"));
    actionFullPath->setCheckable(true);
    actionFullPath->setToolTip(tr("Show document path in the tab caption"));
    connect(this, &MdiWindow::checkedActionFullPath, actionFullPath, &QAction::setChecked);
    connect(actionFullPath, &QAction::toggled, this, &MdiWindow::onActionFullPathToggled);

    auto *actionCopyFilePath = new QAction(tr("Cop&y File Path"), this);
    actionCopyFilePath->setObjectName(QStringLiteral("actionCopyFilePath"));
    actionCopyFilePath->setIcon(QIcon::fromTheme(QStringLiteral("edit-copy"), QIcon(QStringLiteral(":/icons/actions/16/edit-copy.svg"))));
    actionCopyFilePath->setToolTip(tr("Copy file path of the document to clipboard"));
    connect(this, &MdiWindow::enableActionCopyFilePath, actionCopyFilePath, &QAction::setEnabled);
    connect(actionCopyFilePath, &QAction::triggered, this, &MdiWindow::onActionCopyFilePathTriggered);

    menu->clear();
    menu->addAction(actionClose);
    menu->addAction(actionCloseOther);
    menu->addSeparator();
    menu->addAction(actionFullPath);
    menu->addAction(actionCopyFilePath);
}


void MdiWindow::updateWindowIcon(const bool modified)
{
    QIcon icon = modified ? QIcon::fromTheme(QStringLiteral("document-save"), QIcon(QStringLiteral(":/icons/actions/16/document-save.svg"))) : QIcon();

    setWindowIcon(icon);
}
