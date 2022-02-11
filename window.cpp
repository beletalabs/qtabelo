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
        m_toolbarDocument->setVisible(true);
        m_toolbarEdit->setVisible(true);
        m_toolbarTools->setVisible(true);
        m_toolbarFormats->setVisible(true);
        m_toolbarView->setVisible(false);
        m_toolbarAppearance->setVisible(false);
        m_toolbarHelp->setVisible(false);
    }

    // Application property: Status Bar
    const auto visible = settings.value(QStringLiteral("Application/StatusBar"), true).toBool();
    m_statusbar->setVisible(visible);
    m_actionStatusbar->setChecked(visible);

    // Application property: Tool Button Style
    const auto style = settings.value(QStringLiteral("Application/ToolButtonStyle"), static_cast<int>(Qt::ToolButtonFollowStyle)).toInt();
    updateActionsToolButtonStyle(static_cast<Qt::ToolButtonStyle>(style));
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

    // Application property: Tool Button Style
    const auto style = m_actionsToolButtonStyle->checkedAction()->data();
    settings.setValue(QStringLiteral("Application/ToolButtonStyle"), style);
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

    m_actionToolbarDocument = new QAction(tr("Show Document Toolbar"), this);
    m_actionToolbarDocument->setObjectName(QStringLiteral("actionToolbarDocument"));
    m_actionToolbarDocument->setCheckable(true);
    m_actionToolbarDocument->setToolTip(tr("Display the Document toolbar"));
    connect(m_actionToolbarDocument, &QAction::toggled, [=] (const bool checked) { m_toolbarDocument->setVisible(checked); });

    m_actionToolbarEdit = new QAction(tr("Show Edit Toolbar"), this);
    m_actionToolbarEdit->setObjectName(QStringLiteral("actionToolbarEdit"));
    m_actionToolbarEdit->setCheckable(true);
    m_actionToolbarEdit->setToolTip(tr("Display the Edit toolbar"));
    connect(m_actionToolbarEdit, &QAction::toggled, [=] (const bool checked) { m_toolbarEdit->setVisible(checked); });

    m_actionToolbarTools = new QAction(tr("Show Tools Toolbar"), this);
    m_actionToolbarTools->setObjectName(QStringLiteral("actionToolbarTools"));
    m_actionToolbarTools->setCheckable(true);
    m_actionToolbarTools->setToolTip(tr("Display the Tools toolbar"));
    connect(m_actionToolbarTools, &QAction::toggled, [=] (const bool checked) { m_toolbarTools->setVisible(checked); });

    m_actionToolbarFormats = new QAction(tr("Show Formats Toolbar"), this);
    m_actionToolbarFormats->setObjectName(QStringLiteral("actionToolbarFormats"));
    m_actionToolbarFormats->setCheckable(true);
    m_actionToolbarFormats->setToolTip(tr("Display the Formats toolbar"));
    connect(m_actionToolbarFormats, &QAction::toggled, [=] (const bool checked) { m_toolbarFormats->setVisible(checked); });

    m_actionToolbarView = new QAction(tr("Show View Toolbar"), this);
    m_actionToolbarView->setObjectName(QStringLiteral("actionToolbarView"));
    m_actionToolbarView->setCheckable(true);
    m_actionToolbarView->setToolTip(tr("Display the View toolbar"));
    connect(m_actionToolbarView, &QAction::toggled, [=] (const bool checked) { m_toolbarView->setVisible(checked); });

    m_actionToolbarAppearance = new QAction(tr("Show Appearance Toolbar"), this);
    m_actionToolbarAppearance->setObjectName(QStringLiteral("actionToolbarAppearance"));
    m_actionToolbarAppearance->setCheckable(true);
    m_actionToolbarAppearance->setToolTip(tr("Display the Appearance toolbar"));
    connect(m_actionToolbarAppearance, &QAction::toggled, [=] (const bool checked) { m_toolbarAppearance->setVisible(checked); });

    m_actionStatusbar = new QAction(tr("Show Status Bar"), this);
    m_actionStatusbar->setObjectName(QStringLiteral("actionStatusbar"));
    m_actionStatusbar->setCheckable(true);
    m_actionStatusbar->setToolTip(tr("Display the Status bar"));
    connect(m_actionStatusbar, &QAction::toggled, [=] (const bool checked) { m_statusbar->setVisible(checked); });


    //
    // Action group: Tool Button Style

    auto *actionToolButtonStyleIconOnly = new QAction(tr("Icon Only"), this);
    actionToolButtonStyleIconOnly->setObjectName(QStringLiteral("actionToolButtonStyleIconOnly"));
    actionToolButtonStyleIconOnly->setCheckable(true);
    actionToolButtonStyleIconOnly->setToolTip(tr("Only display the icon"));
    actionToolButtonStyleIconOnly->setData(static_cast<int>(Qt::ToolButtonIconOnly));

    auto *actionToolButtonStyleTextOnly = new QAction(tr("Text Only"), this);
    actionToolButtonStyleTextOnly->setObjectName(QStringLiteral("actionToolButtonStyleTextOnly"));
    actionToolButtonStyleTextOnly->setCheckable(true);
    actionToolButtonStyleTextOnly->setToolTip(tr("Only display the text"));
    actionToolButtonStyleTextOnly->setData(static_cast<int>(Qt::ToolButtonTextOnly));

    auto *actionToolButtonStyleTextBesideIcon = new QAction(tr("Text Beside Icon"), this);
    actionToolButtonStyleTextBesideIcon->setObjectName(QStringLiteral("actionToolButtonStyleTextBesideIcon"));
    actionToolButtonStyleTextBesideIcon->setCheckable(true);
    actionToolButtonStyleTextBesideIcon->setToolTip(tr("The text appears beside the icon"));
    actionToolButtonStyleTextBesideIcon->setData(static_cast<int>(Qt::ToolButtonTextBesideIcon));

    auto *actionToolButtonStyleTextUnderIcon = new QAction(tr("Text Under Icon"), this);
    actionToolButtonStyleTextUnderIcon->setObjectName(QStringLiteral("actionToolButtonStyleTextUnderIcon"));
    actionToolButtonStyleTextUnderIcon->setCheckable(true);
    actionToolButtonStyleTextUnderIcon->setToolTip(tr("The text appears under the icon"));
    actionToolButtonStyleTextUnderIcon->setData(static_cast<int>(Qt::ToolButtonTextUnderIcon));

    auto *actionToolButtonStyleFollowStyle = new QAction(tr("Follow Style"), this);
    actionToolButtonStyleFollowStyle->setObjectName(QStringLiteral("actionToolButtonStyleFollowStyle"));
    actionToolButtonStyleFollowStyle->setCheckable(true);
    actionToolButtonStyleFollowStyle->setToolTip(tr("Follow the style"));
    actionToolButtonStyleFollowStyle->setData(static_cast<int>(Qt::ToolButtonFollowStyle));

    m_actionsToolButtonStyle = new QActionGroup(this);
    m_actionsToolButtonStyle->setObjectName(QStringLiteral("actionsToolButtonStyle"));
    m_actionsToolButtonStyle->addAction(actionToolButtonStyleIconOnly);
    m_actionsToolButtonStyle->addAction(actionToolButtonStyleTextOnly);
    m_actionsToolButtonStyle->addAction(actionToolButtonStyleTextBesideIcon);
    m_actionsToolButtonStyle->addAction(actionToolButtonStyleTextUnderIcon);
    m_actionsToolButtonStyle->addAction(actionToolButtonStyleFollowStyle);
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

    // Menu: Document
    auto *menuDocument = menuBar()->addMenu(tr("Document"));
    menuDocument->setObjectName(QStringLiteral("menuDocument"));

    // Menu: Edit
    auto *menuEdit = menuBar()->addMenu(tr("Edit"));
    menuEdit->setObjectName(QStringLiteral("menuEdit"));

    // Menu: Tools
    auto *menuTools = menuBar()->addMenu(tr("Tools"));
    menuTools->setObjectName(QStringLiteral("menuTools"));

    // Menu: Formats
    auto *menuFormats = menuBar()->addMenu(tr("Formats"));
    menuFormats->setObjectName(QStringLiteral("menuFormats"));


    //
    // Menus: View

    auto *menuToolbars = new QMenu(tr("Toolbars"), this);
    menuToolbars->setObjectName(QStringLiteral("menuToolbars"));
    menuToolbars->addAction(m_actionToolbarApplication);
    menuToolbars->addAction(m_actionToolbarDocument);
    menuToolbars->addAction(m_actionToolbarEdit);
    menuToolbars->addAction(m_actionToolbarTools);
    menuToolbars->addAction(m_actionToolbarFormats);
    menuToolbars->addAction(m_actionToolbarView);
    menuToolbars->addAction(m_actionToolbarAppearance);
    menuToolbars->addSection(tr("Tool Button Style"));
    menuToolbars->addActions(m_actionsToolButtonStyle->actions());

    auto *menuView = menuBar()->addMenu(tr("View"));
    menuView->setObjectName(QStringLiteral("menuView"));
    menuView->addMenu(menuToolbars);
    menuView->addAction(m_actionStatusbar);


    // Menu: Appearance
    auto *menuAppearance = menuBar()->addMenu(tr("Appearance"));
    menuAppearance->setObjectName(QStringLiteral("menuAppearance"));

    // Menu: Help
    auto *menuHelp = menuBar()->addMenu(tr("Help"));
    menuHelp->setObjectName(QStringLiteral("menuHelp"));
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

    // Toolbar: Document
    m_toolbarDocument = addToolBar(tr("Document"));
    m_toolbarDocument->setObjectName(QStringLiteral("m_toolbarDocument"));
    connect(m_toolbarDocument, &QToolBar::visibilityChanged, [=] (const bool visible) { m_actionToolbarDocument->setChecked(visible); });

    // Toolbar: Edit
    m_toolbarEdit = addToolBar(tr("Edit"));
    m_toolbarEdit->setObjectName(QStringLiteral("toolbarEdit"));
    connect(m_toolbarEdit, &QToolBar::visibilityChanged, [=] (const bool visible) { m_actionToolbarEdit->setChecked(visible); });

    // Toolbar: Tools
    m_toolbarTools = addToolBar(tr("Tools"));
    m_toolbarTools->setObjectName(QStringLiteral("toolbarTools"));
    connect(m_toolbarTools, &QToolBar::visibilityChanged, [=] (const bool visible) { m_actionToolbarTools->setChecked(visible); });

    // Toolbar: Formats
    m_toolbarFormats = addToolBar(tr("Formats"));
    m_toolbarFormats->setObjectName(QStringLiteral("toolbarFormats"));
    connect(m_toolbarFormats, &QToolBar::visibilityChanged, [=] (const bool visible) { m_actionToolbarFormats->setChecked(visible); });

    // Toolbar: View
    m_toolbarView = addToolBar(tr("View"));
    m_toolbarView->setObjectName(QStringLiteral("toolbarView"));
    connect(m_toolbarView, &QToolBar::visibilityChanged, [=] (const bool visible) { m_actionToolbarView->setChecked(visible); });

    // Toolbar: Appearance
    m_toolbarAppearance = addToolBar(tr("Appearance"));
    m_toolbarAppearance->setObjectName(QStringLiteral("toolbarAppearance"));
    connect(m_toolbarAppearance, &QToolBar::visibilityChanged, [=] (const bool visible) { m_actionToolbarAppearance->setChecked(visible); });

    // Toolbar: Help
    m_toolbarHelp = addToolBar(tr("Help"));
    m_toolbarHelp->setObjectName(QStringLiteral("toolbarHelp"));
}


void Window::updateActionsToolButtonStyle(const Qt::ToolButtonStyle toolButtonStyle)
{
    const QList<QAction *> actions = m_actionsToolButtonStyle->actions();
    for (auto *action : actions) {
        if (static_cast<Qt::ToolButtonStyle>(action->data().toInt()) == toolButtonStyle) {
            action->setChecked(true);
            onActionsToolButtonStyleTriggered(action);
            break;
        }
    }
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
