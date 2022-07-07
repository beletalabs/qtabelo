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

#include "colophon_dialog.h"

#include <QDialogButtonBox>
#include <QTabWidget>
#include <QVBoxLayout>

#include "colophon_pages.h"
#include "dialog_header_box.h"


ColophonDialog::ColophonDialog(QWidget *parent)
    : QDialog{parent}
{
    setMinimumSize(640, 480);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setWindowTitle(tr("Colophon"));

    // Header box
    auto *headerBox = new DialogHeaderBox;


    //
    // Content

    auto *pageAbout = new ColophonPageAbout;
    auto *pageEnvironment = new ColophonPageEnvironment;
    auto *pageAuthors = new ColophonPageAuthors;
    auto *pageCredits = new ColophonPageCredits;
    auto *pageLicense = new ColophonPageLicense;

    auto *tabBox = new QTabWidget;
    tabBox->addTab(pageAbout, pageAbout->title());
    tabBox->addTab(pageEnvironment, pageEnvironment->title());
    tabBox->addTab(pageAuthors, pageAuthors->title());
    tabBox->addTab(pageCredits, pageCredits->title());
    tabBox->addTab(pageLicense, pageLicense->title());


    // Button box
    auto *buttonBox = new QDialogButtonBox(QDialogButtonBox::Close);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &ColophonDialog::close);

    // Main layout
    auto *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(headerBox);
    mainLayout->addWidget(tabBox);
    mainLayout->addWidget(buttonBox);
    setLayout(mainLayout);
}
