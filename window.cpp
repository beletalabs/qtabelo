/**
 * Copyright 2022 Tabelo, <https://github.com/tabeloapp>.
 *
 * This file is part of Tabelo-Qt, <https://github.com/tabeloapp/tabelo-qt>.
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

#include "window.h"

#include <QApplication>
#include <QMenuBar>
#include <QScreen>
#include <QSettings>

#include "about_dialog.h"


Window::Window(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowIcon(QIcon(QStringLiteral(":/icons/apps/16/tabelo.svg")));

    createActions();
    createMenuBar();
    createStatusBar();
    createToolBars();

    loadSettings();
}

Window::~Window()
{
}


void Window::closeEvent(QCloseEvent *event)
{
    if (true) {
        // Store application properties
        saveSettings();

        event->accept();
    }
    else {
        event->ignore();
    }
}


void Window::loadSettings()
{
    QSettings settings;

    // Application property: Geometry
    const auto geometry = settings.value(QStringLiteral("Application/Geometry"), QByteArray()).toByteArray();
    if (!geometry.isEmpty()) {
        restoreGeometry(geometry);
    }
    else {
        // Default: Center window
        const auto availableGeometry = screen()->availableGeometry();
        resize(availableGeometry.width() * 2/3, availableGeometry.height() * 2/3);
        move((availableGeometry.width() - width()) / 2, (availableGeometry.height() - height()) / 2);
    }

    // Application property: State
    const auto state = settings.value(QStringLiteral("Application/State"), QByteArray()).toByteArray();
    if (!state.isEmpty()) {
        restoreState(state);
    }
    else {
        // Default: Show/hide toolbars
        m_toolbarApplication->setVisible(true);
        m_toolbarView->setVisible(false);
    }

    // Application property: Status Bar
    const auto visible = settings.value(QStringLiteral("Application/StatusBar"), true).toBool();
    m_statusbar->setVisible(visible);
    m_actionStatusbar->setChecked(visible);
}


void Window::saveSettings()
{
    QSettings settings;

    // Application property: Geometry
    const auto geometry = saveGeometry();
    settings.setValue(QStringLiteral("Application/Geometry"), geometry);

    // Application property: State
    const auto state = saveState();
    settings.setValue(QStringLiteral("Application/State"), state);

    // Application property: Status Bar
    const auto visible = m_statusbar->isVisible();
    settings.setValue(QStringLiteral("Application/StatusBar"), visible);
}


void Window::createActions()
{
    //
    // Actions: Application

    m_actionAbout = new QAction(tr("About %1").arg(QApplication::applicationName()), this);
    m_actionAbout->setObjectName(QStringLiteral("actionAbout"));
    m_actionAbout->setIcon(QIcon(QStringLiteral(":/icons/apps/16/tabelo.svg")));
    m_actionAbout->setIconText(tr("About"));
    m_actionAbout->setToolTip(tr("Brief description of the application"));
    connect(m_actionAbout, &QAction::triggered, this, &Window::onActionAboutTriggered);

    m_actionQuit = new QAction(tr("Quit"), this);
    m_actionQuit->setObjectName(QStringLiteral("actionQuit"));
    m_actionQuit->setIcon(QIcon::fromTheme(QStringLiteral("application-exit"), QIcon(QStringLiteral(":/icons/actions/16/application-exit.svg"))));
    m_actionQuit->setShortcut(QKeySequence::Quit);
    m_actionQuit->setToolTip(tr("Quit the application"));
    connect(m_actionQuit, &QAction::triggered, this, &Window::close);


    //
    // Actions: View

    m_actionToolbarApplication = new QAction(tr("Show Application Toolbar"), this);
    m_actionToolbarApplication->setObjectName(QStringLiteral("actionToolbarApplication"));
    m_actionToolbarApplication->setCheckable(true);
    m_actionToolbarApplication->setToolTip(tr("Display the Application toolbar"));
    connect(m_actionToolbarApplication, &QAction::toggled, [=] (const bool checked) { m_toolbarApplication->setVisible(checked); });

    m_actionToolbarView = new QAction(tr("Show View Toolbar"), this);
    m_actionToolbarView->setObjectName(QStringLiteral("actionToolbarView"));
    m_actionToolbarView->setCheckable(true);
    m_actionToolbarView->setToolTip(tr("Display the View toolbar"));
    connect(m_actionToolbarView, &QAction::toggled, [=] (const bool checked) { m_toolbarView->setVisible(checked); });

    m_actionStatusbar = new QAction(tr("Show Status Bar"), this);
    m_actionStatusbar->setObjectName(QStringLiteral("actionStatusbar"));
    m_actionStatusbar->setCheckable(true);
    m_actionStatusbar->setToolTip(tr("Display the Status bar"));
    connect(m_actionStatusbar, &QAction::toggled, [=] (const bool checked) { m_statusbar->setVisible(checked); });


    //
    // Action group: Tool Button Style

    m_actionsToolButtonStyle = new QActionGroup(this);
    m_actionsToolButtonStyle->setObjectName(QStringLiteral("actionsToolButtonStyle"));
    connect(m_actionsToolButtonStyle, &QActionGroup::triggered, this, &Window::onActionsToolButtonStyleTriggered);
}


void Window::createMenuBar()
{
    // Menu: Application
    auto *menuApplication = menuBar()->addMenu(tr("Application"));
    menuApplication->setObjectName(QStringLiteral("menuApplication"));
    menuApplication->addAction(m_actionAbout);
    menuApplication->addSeparator();
    menuApplication->addAction(m_actionQuit);


    //
    // Menus: View

    auto *menuToolbars = new QMenu(tr("Toolbars"), this);
    menuToolbars->setObjectName(QStringLiteral("menuToolbars"));
    menuToolbars->addAction(m_actionToolbarApplication);
    menuToolbars->addAction(m_actionToolbarView);
    menuToolbars->addSection(tr("Tool Button Style"));
    menuToolbars->addActions(m_actionsToolButtonStyle->actions());

    auto *menuView = menuBar()->addMenu(tr("View"));
    menuView->setObjectName(QStringLiteral("menuView"));
    menuView->addMenu(menuToolbars);
    menuView->addAction(m_actionStatusbar);
}


void Window::createStatusBar()
{
    m_statusbar = statusBar();
    m_statusbar->showMessage(tr("Ready"), 3000);
}


void Window::createToolBars()
{
    // Toolbar: Application
    m_toolbarApplication = addToolBar(tr("Application"));
    m_toolbarApplication->setObjectName(QStringLiteral("toolbarApplication"));
    m_toolbarApplication->addAction(m_actionAbout);
    m_toolbarApplication->addSeparator();
    m_toolbarApplication->addAction(m_actionQuit);
    connect(m_toolbarApplication, &QToolBar::visibilityChanged, [=] (const bool visible) { m_actionToolbarApplication->setChecked(visible); });

    // Toolbar: View
    m_toolbarView = addToolBar(tr("View"));
    m_toolbarView->setObjectName(QStringLiteral("toolbarView"));
    connect(m_toolbarView, &QToolBar::visibilityChanged, [=] (const bool visible) { m_actionToolbarView->setChecked(visible); });
}


void Window::onActionAboutTriggered()
{
    auto *dialog = new AboutDialog(this);
    dialog->setWindowModality(Qt::ApplicationModal);
    dialog->show();
}


void Window::onActionsToolButtonStyleTriggered(const QAction *actionToolButtonStyle)
{
    const auto style = static_cast<Qt::ToolButtonStyle>(actionToolButtonStyle->data().toInt());

    m_toolbarApplication->setToolButtonStyle(style);
    m_toolbarView->setToolButtonStyle(style);
}
