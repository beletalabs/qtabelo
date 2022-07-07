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

#include "about_dialog.h"

#include <QApplication>
#include <QDialogButtonBox>
#include <QVBoxLayout>

#include "colophon_pages.h"
#include "dialog_header_box.h"


AboutDialog::AboutDialog(QWidget *parent)
    : QDialog{parent}
{
    setMinimumSize(480, 320);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setWindowTitle(tr("About %1").arg(QApplication::applicationName()));

    // Header box
    auto *headerBox = new DialogHeaderBox;

    // Content
    auto *pageAbout = new ColophonPageAbout;
    pageAbout->layout()->setContentsMargins(0, 0, 0, 0);

    // Button box
    auto *buttonBox = new QDialogButtonBox(QDialogButtonBox::Close);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &AboutDialog::close);

    // Main layout
    auto *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(headerBox);
    mainLayout->addWidget(pageAbout);
    mainLayout->addWidget(buttonBox);
    setLayout(mainLayout);
}
