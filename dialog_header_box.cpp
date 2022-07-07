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

#include "dialog_header_box.h"

#include <QApplication>
#include <QGridLayout>
#include <QLabel>
#include <QSvgWidget>


DialogHeaderBox::DialogHeaderBox(QWidget *parent)
    : QWidget{parent}
{
    auto *logo = new QSvgWidget;
    logo->load(QStringLiteral(":/icons/apps/22/tabelo.svg"));

    auto *title = new QLabel(QStringLiteral("<strong style=\"font-size:large;\">%1</strong>").arg(QApplication::applicationName()));
    auto *version = new QLabel(QApplication::applicationVersion());
    auto *subtitle = new QLabel(tr("A table editor based on Qt for C++"));

    // Main layout
    auto *mainLayout = new QGridLayout;
    mainLayout->addWidget(logo, 0, 0, 2, 1);
    mainLayout->addWidget(title, 0, 1, 1, 1);
    mainLayout->addWidget(version, 0, 2, 1, 1, Qt::AlignBottom);
    mainLayout->addWidget(subtitle, 1, 1, 1, 2);
    mainLayout->setColumnStretch(2, 1);
    setLayout(mainLayout);

    // Logo size
    const int height = title->sizeHint().height() + mainLayout->verticalSpacing() + subtitle->sizeHint().height();
    logo->setFixedSize(height, height);
}
