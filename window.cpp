/**
 * Copyright 2022 naracanto <https://naracanto.github.io>.
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
#include "colophon_dialog.h"
#include "preferences_dialog.h"


Window::Window(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowIcon(QIcon(QStringLiteral(":/icons/apps/16/tabelo.svg")));

    createActions();
    createMenuBar();
    createStatusBar();
    createToolBars();

    loadSettings();

    updateActionFullScreen();
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
        m_toolbarFile->setVisible(true);
        m_toolbarEdit->setVisible(true);
        m_toolbarView->setVisible(true);
        m_toolbarFormat->setVisible(true);
        m_toolbarTools->setVisible(true);
        m_toolbarAppearance->setVisible(false);
        m_toolbarHelp->setVisible(false);
    }

    // Application property: Menu Bar
    const auto visibleMenuBar = settings.value(QStringLiteral("Application/MenuBar"), true).toBool();
    menuBar()->setVisible(visibleMenuBar);
    m_actionMenubar->setChecked(visibleMenuBar);

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

    // Application property: Menu Bar
    const auto visibleMenuBar = menuBar()->isVisible();
    settings.setValue(QStringLiteral("Application/MenuBar"), visibleMenuBar);

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

    m_actionColophon = new QAction(tr("Colophon"), this);
    m_actionColophon->setObjectName(QStringLiteral("actionColophon"));
    m_actionColophon->setToolTip(tr("Lengthy description of the application"));
    connect(m_actionColophon, &QAction::triggered, this, &Window::onActionColophonTriggered);

    m_actionPreferences = new QAction(tr("Preferences…"), this);
    m_actionPreferences->setObjectName(QStringLiteral("actionPreferences"));
    m_actionPreferences->setIcon(QIcon::fromTheme(QStringLiteral("configure"), QIcon(QStringLiteral(":/icons/actions/16/configure.svg"))));
    m_actionPreferences->setToolTip(tr("Customize the appearance and behavior of the application"));
    connect(m_actionPreferences, &QAction::triggered, this, &Window::onActionPreferencesTriggered);

    m_actionQuit = new QAction(tr("Quit"), this);
    m_actionQuit->setObjectName(QStringLiteral("actionQuit"));
    m_actionQuit->setIcon(QIcon::fromTheme(QStringLiteral("application-exit"), QIcon(QStringLiteral(":/icons/actions/16/application-exit.svg"))));
    m_actionQuit->setShortcut(QKeySequence::Quit);
    m_actionQuit->setToolTip(tr("Quit the application"));
    connect(m_actionQuit, &QAction::triggered, this, &Window::close);
    addAction(m_actionQuit);


    //
    // Actions: Appearance

    m_actionMenubar = new QAction(tr("Show Menu Bar"), this);
    m_actionMenubar->setObjectName(QStringLiteral("actionMenubar"));
    m_actionMenubar->setCheckable(true);
    m_actionMenubar->setIcon(QIcon::fromTheme(QStringLiteral("show-menu"), QIcon(QStringLiteral(":/icons/actions/16/show-menu.svg"))));
    m_actionMenubar->setIconText(tr("Menu Bar"));
    m_actionMenubar->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_M));
    m_actionMenubar->setToolTip(tr("Display the Menu bar"));
    connect(m_actionMenubar, &QAction::toggled, this, [=] (const bool checked) { menuBar()->setVisible(checked); });
    addAction(m_actionMenubar);

    m_actionToolbarApplication = new QAction(tr("Show Application Toolbar"), this);
    m_actionToolbarApplication->setObjectName(QStringLiteral("actionToolbarApplication"));
    m_actionToolbarApplication->setCheckable(true);
    m_actionToolbarApplication->setToolTip(tr("Display the Application toolbar"));
    connect(m_actionToolbarApplication, &QAction::toggled, this, [=] (const bool checked) { m_toolbarApplication->setVisible(checked); });

    m_actionToolbarFile = new QAction(tr("Show File Toolbar"), this);
    m_actionToolbarFile->setObjectName(QStringLiteral("actionToolbarFile"));
    m_actionToolbarFile->setCheckable(true);
    m_actionToolbarFile->setToolTip(tr("Display the File toolbar"));
    connect(m_actionToolbarFile, &QAction::toggled, this, [=] (const bool checked) { m_toolbarFile->setVisible(checked); });

    m_actionToolbarEdit = new QAction(tr("Show Edit Toolbar"), this);
    m_actionToolbarEdit->setObjectName(QStringLiteral("actionToolbarEdit"));
    m_actionToolbarEdit->setCheckable(true);
    m_actionToolbarEdit->setToolTip(tr("Display the Edit toolbar"));
    connect(m_actionToolbarEdit, &QAction::toggled, this, [=] (const bool checked) { m_toolbarEdit->setVisible(checked); });

    m_actionToolbarView = new QAction(tr("Show View Toolbar"), this);
    m_actionToolbarView->setObjectName(QStringLiteral("actionToolbarView"));
    m_actionToolbarView->setCheckable(true);
    m_actionToolbarView->setToolTip(tr("Display the View toolbar"));
    connect(m_actionToolbarView, &QAction::toggled, this, [=] (const bool checked) { m_toolbarView->setVisible(checked); });

    m_actionToolbarFormat = new QAction(tr("Show Format Toolbar"), this);
    m_actionToolbarFormat->setObjectName(QStringLiteral("actionToolbarFormat"));
    m_actionToolbarFormat->setCheckable(true);
    m_actionToolbarFormat->setToolTip(tr("Display the Format toolbar"));
    connect(m_actionToolbarFormat, &QAction::toggled, this, [=] (const bool checked) { m_toolbarFormat->setVisible(checked); });

    m_actionToolbarTools = new QAction(tr("Show Tools Toolbar"), this);
    m_actionToolbarTools->setObjectName(QStringLiteral("actionToolbarTools"));
    m_actionToolbarTools->setCheckable(true);
    m_actionToolbarTools->setToolTip(tr("Display the Tools toolbar"));
    connect(m_actionToolbarTools, &QAction::toggled, this, [=] (const bool checked) { m_toolbarTools->setVisible(checked); });

    m_actionToolbarAppearance = new QAction(tr("Show Appearance Toolbar"), this);
    m_actionToolbarAppearance->setObjectName(QStringLiteral("actionToolbarAppearance"));
    m_actionToolbarAppearance->setCheckable(true);
    m_actionToolbarAppearance->setToolTip(tr("Display the Appearance toolbar"));
    connect(m_actionToolbarAppearance, &QAction::toggled, this, [=] (const bool checked) { m_toolbarAppearance->setVisible(checked); });

    m_actionToolbarHelp = new QAction(tr("Show Help Toolbar"), this);
    m_actionToolbarHelp->setObjectName(QStringLiteral("actionToolbarHelp"));
    m_actionToolbarHelp->setCheckable(true);
    m_actionToolbarHelp->setToolTip(tr("Display the Help toolbar"));
    connect(m_actionToolbarHelp, &QAction::toggled, this, [=] (const bool checked) { m_toolbarHelp->setVisible(checked); });

    m_actionStatusbar = new QAction(tr("Show Status Bar"), this);
    m_actionStatusbar->setObjectName(QStringLiteral("actionStatusbar"));
    m_actionStatusbar->setCheckable(true);
    m_actionStatusbar->setToolTip(tr("Display the Status bar"));
    connect(m_actionStatusbar, &QAction::toggled, this, [=] (const bool checked) { m_statusbar->setVisible(checked); });

    m_actionFullScreen = new QAction(this);
    m_actionFullScreen->setObjectName(QStringLiteral("actionFullScreen"));
    m_actionFullScreen->setCheckable(true);
    m_actionFullScreen->setIconText(tr("Full Screen"));
    m_actionFullScreen->setShortcuts(QList<QKeySequence>() << QKeySequence(Qt::Key_F11) << QKeySequence::FullScreen);
    connect(m_actionFullScreen, &QAction::triggered, this, &Window::onActionFullScreenTriggered);
    addAction(m_actionFullScreen);


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
    menuApplication->addAction(m_actionColophon);
    menuApplication->addSeparator();
    menuApplication->addAction(m_actionPreferences);
    menuApplication->addSeparator();
    menuApplication->addAction(m_actionQuit);

    // Menu: File
    auto *menuFile = menuBar()->addMenu(tr("File"));
    menuFile->setObjectName(QStringLiteral("menuFile"));

    // Menu: Edit
    auto *menuEdit = menuBar()->addMenu(tr("Edit"));
    menuEdit->setObjectName(QStringLiteral("menuEdit"));

    // Menu: View
    auto *menuView = menuBar()->addMenu(tr("View"));
    menuView->setObjectName(QStringLiteral("menuView"));

    // Menu: Format
    auto *menuFormat = menuBar()->addMenu(tr("Format"));
    menuFormat->setObjectName(QStringLiteral("menuFormat"));

    // Menu: Tools
    auto *menuTools = menuBar()->addMenu(tr("Tools"));
    menuTools->setObjectName(QStringLiteral("menuTools"));


    //
    // Menus: Appearance

    auto *menuToolButtonStyle = new QMenu(tr("Tool Button Style"), this);
    menuToolButtonStyle->setObjectName(QStringLiteral("menuToolButtonStyle"));
    menuToolButtonStyle->addActions(m_actionsToolButtonStyle->actions());

    auto *menuAppearance = menuBar()->addMenu(tr("Appearance"));
    menuAppearance->setObjectName(QStringLiteral("menuAppearance"));
    menuAppearance->addAction(m_actionMenubar);
    menuAppearance->addSeparator();
    menuAppearance->addAction(m_actionToolbarApplication);
    menuAppearance->addAction(m_actionToolbarFile);
    menuAppearance->addAction(m_actionToolbarEdit);
    menuAppearance->addAction(m_actionToolbarView);
    menuAppearance->addAction(m_actionToolbarFormat);
    menuAppearance->addAction(m_actionToolbarTools);
    menuAppearance->addAction(m_actionToolbarAppearance);
    menuAppearance->addAction(m_actionToolbarHelp);
    menuAppearance->addMenu(menuToolButtonStyle);
    menuAppearance->addSeparator();
    menuAppearance->addAction(m_actionStatusbar);
    menuAppearance->addSeparator();
    menuAppearance->addAction(m_actionFullScreen);


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
    m_toolbarApplication = addToolBar(tr("Application Toolbar"));
    m_toolbarApplication->setObjectName(QStringLiteral("toolbarApplication"));
    m_toolbarApplication->addAction(m_actionAbout);
    m_toolbarApplication->addAction(m_actionPreferences);
    m_toolbarApplication->addSeparator();
    m_toolbarApplication->addAction(m_actionQuit);
    connect(m_toolbarApplication, &QToolBar::visibilityChanged, this, [=] (const bool visible) { m_actionToolbarApplication->setChecked(visible); });

    // Toolbar: File
    m_toolbarFile = addToolBar(tr("File Toolbar"));
    m_toolbarFile->setObjectName(QStringLiteral("toolbarFile"));
    connect(m_toolbarFile, &QToolBar::visibilityChanged, this, [=] (const bool visible) { m_actionToolbarFile->setChecked(visible); });

    // Toolbar: Edit
    m_toolbarEdit = addToolBar(tr("Edit Toolbar"));
    m_toolbarEdit->setObjectName(QStringLiteral("toolbarEdit"));
    connect(m_toolbarEdit, &QToolBar::visibilityChanged, this, [=] (const bool visible) { m_actionToolbarEdit->setChecked(visible); });

    // Toolbar: View
    m_toolbarView = addToolBar(tr("View Toolbar"));
    m_toolbarView->setObjectName(QStringLiteral("toolbarView"));
    connect(m_toolbarView, &QToolBar::visibilityChanged, this, [=] (const bool visible) { m_actionToolbarView->setChecked(visible); });

    // Toolbar: Format
    m_toolbarFormat = addToolBar(tr("Format Toolbar"));
    m_toolbarFormat->setObjectName(QStringLiteral("toolbarFormat"));
    connect(m_toolbarFormat, &QToolBar::visibilityChanged, this, [=] (const bool visible) { m_actionToolbarFormat->setChecked(visible); });

    // Toolbar: Tools
    m_toolbarTools = addToolBar(tr("Tools Toolbar"));
    m_toolbarTools->setObjectName(QStringLiteral("toolbarTools"));
    connect(m_toolbarTools, &QToolBar::visibilityChanged, this, [=] (const bool visible) { m_actionToolbarTools->setChecked(visible); });

    // Toolbar: Appearance
    m_toolbarAppearance = addToolBar(tr("Appearance Toolbar"));
    m_toolbarAppearance->setObjectName(QStringLiteral("toolbarAppearance"));
    m_toolbarAppearance->addAction(m_actionMenubar);
    m_toolbarAppearance->addSeparator();
    m_toolbarAppearance->addAction(m_actionFullScreen);
    connect(m_toolbarAppearance, &QToolBar::visibilityChanged, this, [=] (const bool visible) { m_actionToolbarAppearance->setChecked(visible); });

    // Toolbar: Help
    m_toolbarHelp = addToolBar(tr("Help Toolbar"));
    m_toolbarHelp->setObjectName(QStringLiteral("toolbarHelp"));
    connect(m_toolbarHelp, &QToolBar::visibilityChanged, this, [=] (const bool visible) { m_actionToolbarHelp->setChecked(visible); });
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


void Window::updateActionFullScreen()
{
    if (!isFullScreen()) {
        m_actionFullScreen->setText(tr("Full Screen Mode"));
        m_actionFullScreen->setChecked(false);
        m_actionFullScreen->setIcon(QIcon::fromTheme(QStringLiteral("view-fullscreen"), QIcon(QStringLiteral(":/icons/actions/16/view-fullscreen.svg"))));
        m_actionFullScreen->setToolTip(tr("Display the window in full screen"));
    }
    else {
        m_actionFullScreen->setText(tr("Exit Full Screen Mode"));
        m_actionFullScreen->setChecked(true);
        m_actionFullScreen->setIcon(QIcon::fromTheme(QStringLiteral("view-restore"), QIcon(QStringLiteral(":/icons/actions/16/view-restore.svg"))));
        m_actionFullScreen->setToolTip(tr("Exit the full screen mode"));
    }
}


void Window::onActionAboutTriggered()
{
    auto *dialog = new AboutDialog(this);
    dialog->open();
}


void Window::onActionColophonTriggered()
{
    auto *dialog = new ColophonDialog(this);
    dialog->open();
}


void Window::onActionPreferencesTriggered()
{
    auto *dialog = new PreferencesDialog(this);
    dialog->open();
}


void Window::onActionsToolButtonStyleTriggered(const QAction *actionToolButtonStyle)
{
    const auto style = static_cast<Qt::ToolButtonStyle>(actionToolButtonStyle->data().toInt());

    m_toolbarApplication->setToolButtonStyle(style);
    m_toolbarFile->setToolButtonStyle(style);
    m_toolbarEdit->setToolButtonStyle(style);
    m_toolbarView->setToolButtonStyle(style);
    m_toolbarFormat->setToolButtonStyle(style);
    m_toolbarTools->setToolButtonStyle(style);
    m_toolbarAppearance->setToolButtonStyle(style);
    m_toolbarHelp->setToolButtonStyle(style);
}


void Window::onActionFullScreenTriggered()
{
    if (!isFullScreen())
        setWindowState(windowState() | Qt::WindowFullScreen);
    else
        setWindowState(windowState() & ~Qt::WindowFullScreen);

    updateActionFullScreen();
}
