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

#include "confirmation_dialog.h"

#include <QCheckBox>
#include <QPushButton>
#include <QSettings>


ConfirmationDialog::ConfirmationDialog(const QString &confirmationKey, QWidget *parent)
    : QMessageBox(parent)
    , m_cbDoNotShowAgain{nullptr}
{
    m_confirmationKey = confirmationKey;

    if (!m_confirmationKey.isEmpty()) {
        m_cbDoNotShowAgain = new QCheckBox(tr("Do not show again"), this);
        setCheckBox(m_cbDoNotShowAgain);
    }
}


QMessageBox::StandardButton ConfirmationDialog::information(QWidget *parent,
                                                            const QString &title, const QString &text,
                                                            StandardButtons buttons, StandardButton defaultButton,
                                                            const QString &confirmationKey)
{
    QSettings settings(parent);

    if (!confirmationKey.isEmpty()) {
        const bool confirm = settings.value(QStringLiteral("Confirmations/") + confirmationKey, true).toBool();
        if (!confirm)
            return QMessageBox::NoButton;
    }

    ConfirmationDialog dialog(confirmationKey, parent);
    dialog.setIcon(QMessageBox::Information);
    dialog.setWindowTitle(title);
    dialog.setText(text);
    dialog.setStandardButtons(buttons);
    dialog.setDefaultButton(defaultButton);

    return dialog.execute();
}


QMessageBox::StandardButton ConfirmationDialog::informationContinueCancel(QWidget *parent,
                                                                          const QString &title, const QString &text,
                                                                          const QString &confirmationKey)
{
    QSettings settings(parent);

    if (!confirmationKey.isEmpty()) {
        const bool confirm = settings.value(QStringLiteral("Confirmations/") + confirmationKey, true).toBool();
        if (!confirm)
            return QMessageBox::NoButton;
    }

    ConfirmationDialog dialog(confirmationKey, parent);
    dialog.setIcon(QMessageBox::Information);
    dialog.setWindowTitle(title);
    dialog.setText(text);
    QPushButton *buttonContinue = dialog.addButton(tr("C&ontinue"), QMessageBox::AcceptRole);
    buttonContinue->setIcon(QIcon::fromTheme(QStringLiteral("go-next"), QIcon(QStringLiteral(":/icons/actions/16/go-next.svg"))));
    dialog.addButton(QMessageBox::Cancel);
    dialog.setDefaultButton(buttonContinue);

    return dialog.execute();
}


QMessageBox::StandardButton ConfirmationDialog::warning(QWidget *parent,
                                                        const QString &title, const QString &text,
                                                        StandardButtons buttons, StandardButton defaultButton,
                                                        const QString &confirmationKey)
{
    QSettings settings(parent);

    if (!confirmationKey.isEmpty()) {
        const bool confirm = settings.value(QStringLiteral("Confirmations/") + confirmationKey, true).toBool();
        if (!confirm)
            return QMessageBox::NoButton;
    }

    ConfirmationDialog dialog(confirmationKey, parent);
    dialog.setIcon(QMessageBox::Warning);
    dialog.setWindowTitle(title);
    dialog.setText(text);
    dialog.setStandardButtons(buttons);
    dialog.setDefaultButton(defaultButton);

    return dialog.execute();
}


QMessageBox::StandardButton ConfirmationDialog::warningContinueCancel(QWidget *parent,
                                                                      const QString &title, const QString &text,
                                                                      const QString &confirmationKey)
{
    QSettings settings(parent);

    if (!confirmationKey.isEmpty()) {
        const bool confirm = settings.value(QStringLiteral("Confirmations/") + confirmationKey, true).toBool();
        if (!confirm)
            return QMessageBox::NoButton;
    }

    ConfirmationDialog dialog(confirmationKey, parent);
    dialog.setIcon(QMessageBox::Warning);
    dialog.setWindowTitle(title);
    dialog.setText(text);
    QPushButton *buttonContinue = dialog.addButton(tr("C&ontinue"), QMessageBox::AcceptRole);
    buttonContinue->setIcon(QIcon::fromTheme(QStringLiteral("go-next"), QIcon(QStringLiteral(":/icons/actions/16/go-next.svg"))));
    dialog.addButton(QMessageBox::Cancel);
    dialog.setDefaultButton(buttonContinue);

    return dialog.execute();
}


QMessageBox::StandardButton ConfirmationDialog::execute()
{
    QSettings settings;

    exec();

    if (m_cbDoNotShowAgain && m_cbDoNotShowAgain->isChecked())
        settings.setValue(QStringLiteral("Confirmations/") + m_confirmationKey, !m_cbDoNotShowAgain->isChecked());

    return standardButton(clickedButton());
}
