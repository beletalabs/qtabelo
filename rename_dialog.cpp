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

#include "rename_dialog.h"


RenameDialog::RenameDialog(QWidget *parent)
    : QInputDialog(parent)
{

}


QString RenameDialog::getFileName(QWidget *parent, const QString &title, const QString &label,
                                  QLineEdit::EchoMode echo, const QString &text, bool *ok,
                                  Qt::WindowFlags flags, Qt::InputMethodHints inputMethodHints)
{
    return QInputDialog::getText(parent, title, label, echo, text, ok, flags, inputMethodHints);
}
