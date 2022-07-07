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

#ifndef CONFIRMATION_DIALOG_H
#define CONFIRMATION_DIALOG_H

#include <QMessageBox>


class ConfirmationDialog : public QMessageBox
{
    Q_OBJECT

public:
    static StandardButton information(QWidget *parent,
                                      const QString &title, const QString &text,
                                      StandardButtons buttons = Ok, StandardButton defaultButton = NoButton,
                                      const QString &confirmationKey = "");

    static StandardButton warning(QWidget *parent,
                                  const QString &title, const QString &text,
                                  StandardButtons buttons = Ok, StandardButton defaultButton = NoButton,
                                  const QString &confirmationKey = "");

    static StandardButton informationContinueCancel(QWidget *parent,
                                                    const QString &title, const QString &text,
                                                    const QString &confirmationKey = "");

    static StandardButton warningContinueCancel(QWidget *parent,
                                                const QString &title, const QString &text,
                                                const QString &confirmationKey = "");

private:
    explicit ConfirmationDialog(const QString &confirmationKey, QWidget *parent = nullptr);

    StandardButton execute();

private:
    QString m_confirmationKey;
    QCheckBox *m_cbDoNotShowAgain;
};

#endif // CONFIRMATION_DIALOG_H
