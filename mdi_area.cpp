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

#include "mdi_area.h"

#include <QDebug>
#include <QList>
#include <QMdiSubWindow>
#include <QUrl>

#include "mdi_document.h"


MdiArea::MdiArea(QWidget *parent)
    : QMdiArea{parent}
{

}


QMdiSubWindow *MdiArea::findSubWindow(const QUrl &url) const
{
    if (url.isEmpty())
        return nullptr;

    const QList<QMdiSubWindow *> subWindows = subWindowList();
    for (auto *subWindow : subWindows) {

        auto *document = qobject_cast<MdiDocument *>(subWindow->widget());
        if (document->documentUrl() == url)
            return subWindow;
    }

    return nullptr;
}


int MdiArea::subWindowCount() const
{
    return subWindowList().size();
}


void MdiArea::closeOtherSubWindows(QMdiSubWindow *givenSubWindow)
{
    QList<QMdiSubWindow *> subWindows = subWindowList();

    if (subWindows.isEmpty())
        return;

    // First remove the subwindow from the list that should not be closed
    if (givenSubWindow)
        if (subWindows.indexOf(givenSubWindow) >= 0)
            subWindows.removeOne(givenSubWindow);
        else
            return;
    else
        subWindows.removeOne(activeSubWindow());

    // Then close all other subwindows
    for (auto *subWindow : subWindows)
        subWindow->close();
}


void MdiArea::updateFilenameSequenceNumber(MdiDocument *document)
{
    if (!document)
        return;

    document->resetFilenameSequenceNumber();
    document->setFilenameSequenceNumber(latestFilenameSequenceNumber(document->documentUrl()) + 1);
}


int MdiArea::latestFilenameSequenceNumber(const QUrl &url) const
{
    int number = 0;

    const QList<QMdiSubWindow *> subWindows = subWindowList();
    for (auto *subWindow : subWindows) {

        auto *document = qobject_cast<MdiDocument *>(subWindow->widget());
        if (document->documentUrl().fileName() == url.fileName())
            if (document->filenameSequenceNumber() > number)
                number = document->filenameSequenceNumber();
    }

    return number;
}
