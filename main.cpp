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

#include <QApplication>
#include <QCommandLineParser>
#include <QDir>
#include <QUrl>

#include "application_window.h"


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setOrganizationName(QStringLiteral("Beleta Labs"));
    app.setOrganizationDomain(QStringLiteral("https://beletalabs.github.io"));
    app.setApplicationName(QStringLiteral("QTabelo"));
    app.setApplicationDisplayName(QStringLiteral("QTabelo"));
    app.setApplicationVersion(QStringLiteral("0.1.0"));

    // Command line
    QCommandLineParser parser;
    parser.setApplicationDescription(QCoreApplication::translate("main", "%1 - A table editor based on Qt for C++").arg(app.applicationName()));
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument(QStringLiteral("urls"), QCoreApplication::translate("main", "Documents to open."), QStringLiteral("[urls...]"));
    parser.process(app);


    //
    // Application window

    ApplicationWindow window;
    window.show();

    const QStringList urls = parser.positionalArguments();
    for(const QString &url : urls)
        window.openDocument(QUrl::fromUserInput(url, QDir::currentPath(), QUrl::AssumeLocalFile));


    return app.exec();
}
