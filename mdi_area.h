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

#ifndef MDI_AREA_H
#define MDI_AREA_H

#include <QMdiArea>

class QMdiSubWindow;
class QUrl;

class MdiDocument;


class MdiArea : public QMdiArea
{
    Q_OBJECT

public:
    explicit MdiArea(QWidget *parent = nullptr);

    QMdiSubWindow *findSubWindow(const QUrl &url) const;

    int subWindowCount() const;

public slots:
    void closeSpecificSubWindow(QMdiSubWindow *subWindow);
    void closeOtherSubWindows(QMdiSubWindow *givenSubWindow = nullptr);

    void updateFilenameSequenceNumber(MdiDocument *document);

private:
    int latestFilenameSequenceNumber(const QUrl &url) const;
};

#endif // MDI_AREA_H
