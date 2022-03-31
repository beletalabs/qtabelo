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

#include <QList>
#include <QMdiSubWindow>
#include <QUrl>
#include <QWidget>

#include "mdi_document.h"
#include "mdi_window.h"


MdiArea::MdiArea(QWidget *parent)
    : QMdiArea{parent}
{

}


int MdiArea::subWindowCount() const
{
    return subWindowList().size();
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


void MdiArea::closeSelectedSubWindow(QMdiSubWindow *subWindow)
{
    if (!subWindow)
        return;

    subWindow->close();
}


void MdiArea::closeOtherSubWindows(QMdiSubWindow *subWindow)
{
    QList<QMdiSubWindow *> subWindows = subWindowList();
    if (subWindows.isEmpty() || !subWindow)
        return;

    // First remove the subwindow from the list that should not be closed
    if (subWindows.indexOf(subWindow) >= 0)
        subWindows.removeOne(subWindow);
    else
        return;

    // Then close all other subwindows
    for (auto *subWindow : subWindows)
        closeSelectedSubWindow(subWindow);
}
