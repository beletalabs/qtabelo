/**
 * Copyright 2022 naracanto <https://naracanto.github.io>.
 *
 * This file is part of Tabelo-Qt <https://github.com/tabelolabs/tabelo-qt>.
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

#include <QApplication>
#include <QCommandLineParser>

#include "window.h"


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setOrganizationName(QStringLiteral("Tabelo Labs"));
    app.setOrganizationDomain(QStringLiteral("https://tabelolabs.github.io"));
    app.setApplicationName(QStringLiteral("Tabelo-Qt"));
    app.setApplicationDisplayName(QStringLiteral("Tabelo-Qt"));
    app.setApplicationVersion(QStringLiteral("0.1.0"));

    // Command line
    QCommandLineParser parser;
    parser.setApplicationDescription(QCoreApplication::translate("main", "%1 - A table editor based on Qt for C++").arg(app.applicationName()));
    parser.addHelpOption();
    parser.addVersionOption();
    parser.process(app);

    Window window;
    window.show();

    return app.exec();
}
