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

#include "colophon_pages.h"

#include <QApplication>
#include <QTextBrowser>
#include <QVBoxLayout>


//
//
// Colophon page: About
//

ColophonPageAbout::ColophonPageAbout(QWidget *parent)
    : QWidget{parent}
{
    QString text = QStringLiteral("<html><body>");
    text += tr("<p>%1 is an open source table editor written in C++ using the Qt framework.</p>").arg(QApplication::applicationName());
    text += tr("<p>Copyright &copy; 2022 <a href=\"%1\" title=\"Visit organization's homepage\">%2</a>.</p>").arg(QApplication::organizationDomain(), QApplication::organizationName());
    text += tr("<p>This application is licensed under the terms of the <a href=\"https://www.gnu.org/licenses/gpl-3.0.en.html\" title=\"Visit license's homepage\">GNU General Public License, version 3</a>.</p>");
    text += QStringLiteral("</body></html>");

    auto *textBox = new QTextBrowser;
    textBox->setHtml(text);
    textBox->setOpenExternalLinks(true);
    textBox->setFrameStyle(QFrame::NoFrame);
    textBox->setStyleSheet(QStringLiteral("background-color:transparent;"));

    // Main layout
    auto *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(textBox);
    setLayout(mainLayout);
}


QString ColophonPageAbout::title() const
{
    return tr("About");
}


//
//
// Colophon page: Authors
//

ColophonPageAuthors::ColophonPageAuthors(QWidget *parent)
    : QWidget{parent}
{
    QString text = QStringLiteral("<html><body><dl>");
    text += tr("<dt><strong>naracanto</strong></dt>");
    text += tr("<dd>Created and developed by <a href=\"https://naracanto.github.io\" title=\"Visit author's homepage\">naracanto</a>.</dd>");
    text += QStringLiteral("</dl></body></html>");

    auto *textBox = new QTextBrowser;
    textBox->setHtml(text);
    textBox->setOpenExternalLinks(true);
    textBox->setFrameStyle(QFrame::NoFrame);
    textBox->setStyleSheet(QStringLiteral("background-color:transparent;"));

    // Main layout
    auto *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(textBox);
    setLayout(mainLayout);
}


QString ColophonPageAuthors::title() const
{
    return tr("Authors");
}


//
//
// Colophon page: Credits
//

ColophonPageCredits::ColophonPageCredits(QWidget *parent)
    : QWidget{parent}
{
    QString text = QStringLiteral("<html><body><dl>");
    text += tr("<dt><strong>BreezeIcons project</strong></dt>");
    text += tr("<dd>Application logo and icons made by <a href=\"https://api.kde.org/frameworks/breeze-icons/html/\" title=\"Visit project's homepage\">BreezeIcons project</a> "
               "from <a href=\"https://kde.org\" title=\"Visit organization's homepage\">KDE</a> are licensed under <a href=\"https://www.gnu.org/licenses/lgpl-3.0.en.html\" title=\"Visit license's homepage\">LGPLv3</a>.</dd>");
    text += QStringLiteral("</dl></body></html>");

    auto *textBox = new QTextBrowser;
    textBox->setHtml(text);
    textBox->setOpenExternalLinks(true);
    textBox->setFrameStyle(QFrame::NoFrame);
    textBox->setStyleSheet(QStringLiteral("background-color:transparent;"));

    // Main layout
    auto *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(textBox);
    setLayout(mainLayout);
}


QString ColophonPageCredits::title() const
{
    return tr("Credits");
}


//
//
// Colophon page: Environment
//

ColophonPageEnvironment::ColophonPageEnvironment(QWidget *parent)
    : QWidget{parent}
{
    QString text = QStringLiteral("<html><body><dl>");
    text += tr("<dt><strong>Application version</strong></dt>");
    text += tr("<dd>%1</dd>").arg(QApplication::applicationVersion());
    text += tr("<dt><strong>Qt for C++ version</strong></dt>");
    text += tr("<dd>Qt %1 (Built against %2)</dd>").arg(qVersion(), QT_VERSION_STR);
    text += tr("<dt><strong>Operation System</strong></dt>");
    text += tr("<dd>%1 (Kernel %2 on %3)</dd>").arg(QSysInfo::prettyProductName(), QSysInfo::kernelVersion(), QSysInfo::currentCpuArchitecture());
    text += QStringLiteral("</dl></body></html>");

    auto *textBox = new QTextBrowser;
    textBox->setHtml(text);
    textBox->setOpenExternalLinks(true);
    textBox->setFrameStyle(QFrame::NoFrame);
    textBox->setStyleSheet(QStringLiteral("background-color:transparent;"));

    // Main layout
    auto *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(textBox);
    setLayout(mainLayout);
}


QString ColophonPageEnvironment::title() const
{
    return tr("Environment");
}


//
//
// Colophon page: License
//

ColophonPageLicense::ColophonPageLicense(QWidget *parent)
    : QWidget{parent}
{
    QString text = QStringLiteral("<html><body>");
    text += tr("<p>%1 is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.</p>").arg(QApplication::applicationName());
    text += tr("<p>%1 is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.</p>").arg(QApplication::applicationName());
    text += tr("<p>You should have received a copy of the GNU General Public License along with %1. If not, see <a href=\"https://www.gnu.org/licenses/\" title=\"Visit license's homepage\">https://www.gnu.org/licenses/</a>.</p>").arg(QApplication::applicationName());
    text += QStringLiteral("</body></html>");

    auto *textBox = new QTextBrowser;
    textBox->setHtml(text);
    textBox->setOpenExternalLinks(true);
    textBox->setFrameStyle(QFrame::NoFrame);
    textBox->setStyleSheet(QStringLiteral("background-color:transparent;"));

    // Main layout
    auto *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(textBox);
    setLayout(mainLayout);
}


QString ColophonPageLicense::title() const
{
    return tr("License");
}
