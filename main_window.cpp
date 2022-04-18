/**
 * Copyright 2022 naracanto <https://naracanto.github.io>.
 *
 * This file is part of QTabelo <https://github.com/cutelabs/qtabelo>.
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

#include "main_window.h"

#include <QAction>
#include <QActionGroup>
#include <QApplication>
#include <QClipboard>
#include <QCloseEvent>
#include <QDebug>
#include <QFileDialog>
#include <QMdiSubWindow>
#include <QMenuBar>
#include <QMessageBox>
#include <QMetaEnum>
#include <QScreen>
#include <QSettings>
#include <QStatusBar>
#include <QToolBar>
#include <QUrl>

#include "about_dialog.h"
#include "colophon_dialog.h"
#include "document_widget.h"
#include "mdi_area.h"
#include "mdi_window.h"
#include "preferences_dialog.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_documentsArea{new MdiArea}
{
    setWindowIcon(QIcon(QStringLiteral(":/icons/apps/16/tabelo.svg")));

    m_documentsArea->setViewMode(MdiArea::TabbedView);
    m_documentsArea->setDocumentMode(true);
    m_documentsArea->setTabsClosable(true);
    m_documentsArea->setTabsMovable(true);
    setCentralWidget(m_documentsArea);

    connect(m_documentsArea, &MdiArea::subWindowActivated, this, &MainWindow::documentActivated);

    setupActions();
    loadSettings();

    documentActivated(nullptr);
    documentClosed();
}

MainWindow::~MainWindow()
{

}


void MainWindow::setupActions()
{
    //
    // Application

    m_actionAbout = new QAction(tr("&About %1").arg(QApplication::applicationName()), this);
    m_actionAbout->setObjectName(QStringLiteral("actionAbout"));
    m_actionAbout->setIcon(QIcon(QStringLiteral(":/icons/apps/16/tabelo.svg")));
    m_actionAbout->setIconText(tr("About"));
    m_actionAbout->setToolTip(tr("Brief description of the application"));
    m_actionAbout->setMenuRole(QAction::AboutRole);
    connect(m_actionAbout, &QAction::triggered, this, &MainWindow::slotAbout);

    m_actionColophon = new QAction(tr("&Colophon"), this);
    m_actionColophon->setObjectName(QStringLiteral("actionColophon"));
    m_actionColophon->setIcon(QIcon::fromTheme(QStringLiteral("help-about"), QIcon(QStringLiteral(":/icons/actions/16/help-about.svg"))));
    m_actionColophon->setToolTip(tr("Lengthy description of the application"));
    m_actionColophon->setMenuRole(QAction::ApplicationSpecificRole);
    connect(m_actionColophon, &QAction::triggered, this, &MainWindow::slotColophon);

    m_actionPreferences = new QAction(tr("&Preferences..."), this);
    m_actionPreferences->setObjectName(QStringLiteral("actionPreferences"));
    m_actionPreferences->setIcon(QIcon::fromTheme(QStringLiteral("configure"), QIcon(QStringLiteral(":/icons/actions/16/configure.svg"))));
    m_actionPreferences->setToolTip(tr("Customize the appearance and behavior of the application"));
    m_actionPreferences->setMenuRole(QAction::PreferencesRole);
    connect(m_actionPreferences, &QAction::triggered, this, &MainWindow::slotPreferences);

    m_actionQuit = new QAction(tr("&Quit"), this);
    m_actionQuit->setObjectName(QStringLiteral("actionQuit"));
    m_actionQuit->setIcon(QIcon::fromTheme(QStringLiteral("application-exit"), QIcon(QStringLiteral(":/icons/actions/16/application-exit.svg"))));
    m_actionQuit->setShortcut(QKeySequence::Quit);
    m_actionQuit->setToolTip(tr("Quit the application"));
    m_actionQuit->setMenuRole(QAction::QuitRole);
    connect(m_actionQuit, &QAction::triggered, this, &MainWindow::close);
    addAction(m_actionQuit);

    auto *menuApplication = menuBar()->addMenu(tr("&Application"));
    menuApplication->setObjectName(QStringLiteral("menuApplication"));
    menuApplication->addAction(m_actionAbout);
    menuApplication->addAction(m_actionColophon);
    menuApplication->addSeparator();
    menuApplication->addAction(m_actionPreferences);
    menuApplication->addSeparator();
    menuApplication->addAction(m_actionQuit);

    m_toolbarApplication = addToolBar(tr("Application Toolbar"));
    m_toolbarApplication->setObjectName(QStringLiteral("toolbarApplication"));
    m_toolbarApplication->addAction(m_actionAbout);
    m_toolbarApplication->addAction(m_actionPreferences);
    m_toolbarApplication->addSeparator();
    m_toolbarApplication->addAction(m_actionQuit);


    //
    // File

    m_actionNew = new QAction(tr("&New"), this);
    m_actionNew->setObjectName(QStringLiteral("actionNew"));
    m_actionNew->setIcon(QIcon::fromTheme(QStringLiteral("document-new"), QIcon(QStringLiteral(":/icons/actions/16/document-new.svg"))));
    m_actionNew->setShortcut(QKeySequence::New);
    m_actionNew->setToolTip(tr("Create new document"));
    connect(m_actionNew, &QAction::triggered, this, &MainWindow::slotNew);
    addAction(m_actionNew);

    m_actionOpen = new QAction(tr("&Open..."), this);
    m_actionOpen->setObjectName(QStringLiteral("actionOpen"));
    m_actionOpen->setIcon(QIcon::fromTheme(QStringLiteral("document-open"), QIcon(QStringLiteral(":/icons/actions/16/document-open.svg"))));
    m_actionOpen->setShortcut(QKeySequence::Open);
    m_actionOpen->setToolTip(tr("Open an existing document"));
    connect(m_actionOpen, &QAction::triggered, this, &MainWindow::slotOpen);
    addAction(m_actionOpen);

    m_actionSave = new QAction(tr("&Save"), this);
    m_actionSave->setObjectName(QStringLiteral("actionSave"));
    m_actionSave->setIcon(QIcon::fromTheme(QStringLiteral("document-save"), QIcon(QStringLiteral(":/icons/actions/16/document-save.svg"))));
    m_actionSave->setShortcut(QKeySequence::Save);
    m_actionSave->setToolTip(tr("Save document"));
    connect(m_actionSave, &QAction::triggered, this, &MainWindow::slotSave);
    addAction(m_actionSave);

    m_actionSaveAs = new QAction(tr("Save &As..."), this);
    m_actionSaveAs->setObjectName(QStringLiteral("actionSaveAs"));
    m_actionSaveAs->setIcon(QIcon::fromTheme(QStringLiteral("document-save-as"), QIcon(QStringLiteral(":/icons/actions/16/document-save-as.svg"))));
    m_actionSaveAs->setShortcut(QKeySequence::SaveAs);
    m_actionSaveAs->setToolTip(tr("Save document under a new name"));
    connect(m_actionSaveAs, &QAction::triggered, this, &MainWindow::slotSaveAs);
    addAction(m_actionSaveAs);

    m_actionSaveCopyAs = new QAction(tr("Save &Copy As..."), this);
    m_actionSaveCopyAs->setObjectName(QStringLiteral("actionSaveCopyAs"));
    m_actionSaveCopyAs->setIcon(QIcon::fromTheme(QStringLiteral("document-save-as"), QIcon(QStringLiteral(":/icons/actions/16/document-save-as.svg"))));
    m_actionSaveCopyAs->setToolTip(tr("Save copy of document under a new name"));
    connect(m_actionSaveCopyAs, &QAction::triggered, this, &MainWindow::slotSaveCopyAs);

    m_actionSaveAll = new QAction(tr("Save A&ll"), this);
    m_actionSaveAll->setObjectName(QStringLiteral("actionSaveAll"));
    m_actionSaveAll->setIcon(QIcon::fromTheme(QStringLiteral("document-save-all"), QIcon(QStringLiteral(":/icons/actions/16/document-save-all.svg"))));
    m_actionSaveAll->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_L));
    m_actionSaveAll->setToolTip(tr("Save all open documents"));
    connect(m_actionSaveAll, &QAction::triggered, this, &MainWindow::slotSaveAll);
    addAction(m_actionSaveAll);

    m_actionCopyPath = new QAction(tr("Cop&y Path"), this);
    m_actionCopyPath->setObjectName(QStringLiteral("actionCopyPath"));
    m_actionCopyPath->setIcon(QIcon::fromTheme(QStringLiteral("edit-copy-path"), QIcon(QStringLiteral(":/icons/actions/16/edit-copy-path.svg"))));
    m_actionCopyPath->setToolTip(tr("Copy document path to clipboard"));
    connect(m_actionCopyPath, &QAction::triggered, this, &MainWindow::slotCopyPath);

    m_actionRenameFilename = new QAction(tr("Re&name..."), this);
    m_actionRenameFilename->setObjectName(QStringLiteral("actionRenameFilename"));
    m_actionRenameFilename->setIcon(QIcon::fromTheme(QStringLiteral("edit-rename"), QIcon(QStringLiteral(":/icons/actions/16/edit-rename.svg"))));
    m_actionRenameFilename->setToolTip(tr("Rename file name of the document"));
    connect(m_actionRenameFilename, &QAction::triggered, this, &MainWindow::slotRenameFilename);

    m_actionClose = new QAction(tr("&Close"), this);
    m_actionClose->setObjectName(QStringLiteral("actionClose"));
    m_actionClose->setIcon(QIcon::fromTheme(QStringLiteral("document-close"), QIcon(QStringLiteral(":/icons/actions/16/document-close.svg"))));
    m_actionClose->setShortcut(QKeySequence::Close);
    m_actionClose->setToolTip(tr("Close document"));
    connect(m_actionClose, &QAction::triggered, m_documentsArea, &MdiArea::closeActiveSubWindow);
    addAction(m_actionClose);

    m_actionCloseOther = new QAction(tr("Close Ot&her"), this);
    m_actionCloseOther->setObjectName(QStringLiteral("actionCloseOther"));
    m_actionCloseOther->setToolTip(tr("Close other open documents"));
    connect(m_actionCloseOther, &QAction::triggered, this, &MainWindow::slotCloseOther);

    m_actionCloseAll = new QAction(tr("Clos&e All"), this);
    m_actionCloseAll->setObjectName(QStringLiteral("actionCloseAll"));
    m_actionCloseAll->setToolTip(tr("Close all open documents"));
    connect(m_actionCloseAll, &QAction::triggered, this, &MainWindow::slotCloseAll);

    auto *menuDocument = menuBar()->addMenu(tr("&Document"));
    menuDocument->setObjectName(QStringLiteral("menuDocument"));
    menuDocument->addAction(m_actionNew);
    menuDocument->addSeparator();
    menuDocument->addAction(m_actionOpen);
    menuDocument->addSeparator();
    menuDocument->addAction(m_actionSave);
    menuDocument->addAction(m_actionSaveAs);
    menuDocument->addAction(m_actionSaveCopyAs);
    menuDocument->addAction(m_actionSaveAll);
    menuDocument->addSeparator();
    menuDocument->addAction(m_actionCopyPath);
    menuDocument->addSeparator();
    menuDocument->addAction(m_actionRenameFilename);
    menuDocument->addSeparator();
    menuDocument->addAction(m_actionClose);
    menuDocument->addAction(m_actionCloseOther);
    menuDocument->addAction(m_actionCloseAll);

    m_toolbarDocument = addToolBar(tr("Document Toolbar"));
    m_toolbarDocument->setObjectName(QStringLiteral("toolbarDocument"));
    m_toolbarDocument->addAction(m_actionNew);
    m_toolbarDocument->addAction(m_actionOpen);
    m_toolbarDocument->addSeparator();
    m_toolbarDocument->addAction(m_actionSave);
    m_toolbarDocument->addAction(m_actionSaveAs);
    m_toolbarDocument->addSeparator();
    m_toolbarDocument->addAction(m_actionClose);


    //
    // Edit

    auto *menuEdit = menuBar()->addMenu(tr("&Edit"));
    menuEdit->setObjectName(QStringLiteral("menuEdit"));

    m_toolbarEdit = addToolBar(tr("Edit Toolbar"));
    m_toolbarEdit->setObjectName(QStringLiteral("toolbarEdit"));


    //
    // View

    auto *menuView = menuBar()->addMenu(tr("&View"));
    menuView->setObjectName(QStringLiteral("menuView"));

    m_toolbarView = addToolBar(tr("View Toolbar"));
    m_toolbarView->setObjectName(QStringLiteral("toolbarView"));


    //
    // Format

    auto *menuFormat = menuBar()->addMenu(tr("&Format"));
    menuFormat->setObjectName(QStringLiteral("menuFormat"));

    m_toolbarFormat = addToolBar(tr("Format Toolbar"));
    m_toolbarFormat->setObjectName(QStringLiteral("toolbarFormat"));


    //
    // Tools

    auto *menuTools = menuBar()->addMenu(tr("&Tools"));
    menuTools->setObjectName(QStringLiteral("menuTools"));

    m_toolbarTools = addToolBar(tr("Tools Toolbar"));
    m_toolbarTools->setObjectName(QStringLiteral("toolbarTools"));


    //
    // Appearance

    m_actionShowPath = new QAction(tr("Show &Path in Titlebar"), this);
    m_actionShowPath->setObjectName(QStringLiteral("actionShowPath"));
    m_actionShowPath->setCheckable(true);
    m_actionShowPath->setChecked(true);
    m_actionShowPath->setIcon(QIcon::fromTheme(QStringLiteral("show-path"), QIcon(QStringLiteral(":/icons/actions/16/show-path.svg"))));
    m_actionShowPath->setIconText(tr("Path"));
    m_actionShowPath->setToolTip(tr("Show document path in the window caption"));
    connect(m_actionShowPath, &QAction::toggled, this, &MainWindow::updateWindowTitle);

    m_actionShowMenubar = new QAction(tr("Show &Menubar"), this);
    m_actionShowMenubar->setObjectName(QStringLiteral("actionShowMenubar"));
    m_actionShowMenubar->setCheckable(true);
    m_actionShowMenubar->setChecked(true);
    m_actionShowMenubar->setIcon(QIcon::fromTheme(QStringLiteral("show-menubar"), QIcon(QStringLiteral(":/icons/actions/16/show-menubar.svg"))));
    m_actionShowMenubar->setIconText(tr("Menubar"));
    m_actionShowMenubar->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_M));
    m_actionShowMenubar->setToolTip(tr("Show the menubar"));
    connect(m_actionShowMenubar, &QAction::toggled, menuBar(), &QMenuBar::setVisible);
    addAction(m_actionShowMenubar);

    m_actionShowToolbarApplication = new QAction(tr("Show &Application Toolbar"), this);
    m_actionShowToolbarApplication->setObjectName(QStringLiteral("actionShowToolbarApplication"));
    m_actionShowToolbarApplication->setCheckable(true);
    m_actionShowToolbarApplication->setToolTip(tr("Show the Application toolbar"));

    m_actionShowToolbarDocument = new QAction(tr("Show &Document Toolbar"), this);
    m_actionShowToolbarDocument->setObjectName(QStringLiteral("actionShowToolbarDocument"));
    m_actionShowToolbarDocument->setCheckable(true);
    m_actionShowToolbarDocument->setToolTip(tr("Show the Document toolbar"));

    m_actionShowToolbarEdit = new QAction(tr("Show &Edit Toolbar"), this);
    m_actionShowToolbarEdit->setObjectName(QStringLiteral("actionShowToolbarEdit"));
    m_actionShowToolbarEdit->setCheckable(true);
    m_actionShowToolbarEdit->setToolTip(tr("Show the Edit toolbar"));

    m_actionShowToolbarView = new QAction(tr("Show &View Toolbar"), this);
    m_actionShowToolbarView->setObjectName(QStringLiteral("actionShowToolbarView"));
    m_actionShowToolbarView->setCheckable(true);
    m_actionShowToolbarView->setToolTip(tr("Show the View toolbar"));

    m_actionShowToolbarFormat = new QAction(tr("Show &Format Toolbar"), this);
    m_actionShowToolbarFormat->setObjectName(QStringLiteral("actionShowToolbarFormat"));
    m_actionShowToolbarFormat->setCheckable(true);
    m_actionShowToolbarFormat->setToolTip(tr("Show the Format toolbar"));

    m_actionShowToolbarTools = new QAction(tr("Show &Tools Toolbar"), this);
    m_actionShowToolbarTools->setObjectName(QStringLiteral("actionShowToolbarTools"));
    m_actionShowToolbarTools->setCheckable(true);
    m_actionShowToolbarTools->setToolTip(tr("Show the Tools toolbar"));

    m_actionShowToolbarSettings = new QAction(tr("Show &Settings Toolbar"), this);
    m_actionShowToolbarSettings->setObjectName(QStringLiteral("actionShowToolbarSettings"));
    m_actionShowToolbarSettings->setCheckable(true);
    m_actionShowToolbarSettings->setToolTip(tr("Show the Settings toolbar"));

    m_actionShowToolbarHelp = new QAction(tr("Show &Help Toolbar"), this);
    m_actionShowToolbarHelp->setObjectName(QStringLiteral("actionShowToolbarHelp"));
    m_actionShowToolbarHelp->setCheckable(true);
    m_actionShowToolbarHelp->setToolTip(tr("Show the Help toolbar"));

    auto *actionToolButtonStyleIconOnly = new QAction(tr("&Icon Only"), this);
    actionToolButtonStyleIconOnly->setObjectName(QStringLiteral("actionToolButtonStyleIconOnly"));
    actionToolButtonStyleIconOnly->setCheckable(true);
    actionToolButtonStyleIconOnly->setToolTip(tr("Only display the icon"));
    actionToolButtonStyleIconOnly->setData(static_cast<int>(Qt::ToolButtonIconOnly));

    auto *actionToolButtonStyleTextOnly = new QAction(tr("&Text Only"), this);
    actionToolButtonStyleTextOnly->setObjectName(QStringLiteral("actionToolButtonStyleTextOnly"));
    actionToolButtonStyleTextOnly->setCheckable(true);
    actionToolButtonStyleTextOnly->setToolTip(tr("Only display the text"));
    actionToolButtonStyleTextOnly->setData(static_cast<int>(Qt::ToolButtonTextOnly));

    auto *actionToolButtonStyleTextBesideIcon = new QAction(tr("Text &Beside Icon"), this);
    actionToolButtonStyleTextBesideIcon->setObjectName(QStringLiteral("actionToolButtonStyleTextBesideIcon"));
    actionToolButtonStyleTextBesideIcon->setCheckable(true);
    actionToolButtonStyleTextBesideIcon->setToolTip(tr("The text appears beside the icon"));
    actionToolButtonStyleTextBesideIcon->setData(static_cast<int>(Qt::ToolButtonTextBesideIcon));

    auto *actionToolButtonStyleTextUnderIcon = new QAction(tr("Text &Under Icon"), this);
    actionToolButtonStyleTextUnderIcon->setObjectName(QStringLiteral("actionToolButtonStyleTextUnderIcon"));
    actionToolButtonStyleTextUnderIcon->setCheckable(true);
    actionToolButtonStyleTextUnderIcon->setToolTip(tr("The text appears under the icon"));
    actionToolButtonStyleTextUnderIcon->setData(static_cast<int>(Qt::ToolButtonTextUnderIcon));

    auto *actionToolButtonStyleDefault = new QAction(tr("&Default"), this);
    actionToolButtonStyleDefault->setObjectName(QStringLiteral("actionToolButtonStyleDefault"));
    actionToolButtonStyleDefault->setCheckable(true);
    actionToolButtonStyleDefault->setToolTip(tr("Follow the theme style"));
    actionToolButtonStyleDefault->setData(static_cast<int>(Qt::ToolButtonFollowStyle));

    m_actionsToolButtonStyle = new QActionGroup(this);
    m_actionsToolButtonStyle->setObjectName(QStringLiteral("actionsToolButtonStyle"));
    m_actionsToolButtonStyle->addAction(actionToolButtonStyleIconOnly);
    m_actionsToolButtonStyle->addAction(actionToolButtonStyleTextOnly);
    m_actionsToolButtonStyle->addAction(actionToolButtonStyleTextBesideIcon);
    m_actionsToolButtonStyle->addAction(actionToolButtonStyleTextUnderIcon);
    m_actionsToolButtonStyle->addAction(actionToolButtonStyleDefault);
    connect(m_actionsToolButtonStyle, &QActionGroup::triggered, this, &MainWindow::slotToolButtonStyle);

    m_actionShowStatusbar = new QAction(tr("Show Stat&usbar"), this);
    m_actionShowStatusbar->setObjectName(QStringLiteral("actionShowStatusbar"));
    m_actionShowStatusbar->setCheckable(true);
    m_actionShowStatusbar->setChecked(true);
    m_actionShowStatusbar->setIcon(QIcon::fromTheme(QStringLiteral("show-statusbar"), QIcon(QStringLiteral(":/icons/actions/16/show-statusbar.svg"))));
    m_actionShowStatusbar->setIconText(tr("Statusbar"));
    m_actionShowStatusbar->setToolTip(tr("Show the statusbar"));
    connect(m_actionShowStatusbar, &QAction::toggled, statusBar(), &QStatusBar::setVisible);

    m_actionFullScreen = new QAction(this);
    m_actionFullScreen->setObjectName(QStringLiteral("actionViewFullScreen"));
    m_actionFullScreen->setCheckable(true);
    m_actionFullScreen->setShortcuts(QList<QKeySequence>() << QKeySequence(Qt::Key_F11) << QKeySequence::FullScreen);
    connect(m_actionFullScreen, &QAction::toggled, this, &MainWindow::slotFullScreen);
    addAction(m_actionFullScreen);
    updateActionFullScreen();

    auto *menuToolButtonStyle = new QMenu(tr("Tool Button St&yle"), this);
    menuToolButtonStyle->setObjectName(QStringLiteral("menuToolButtonStyle"));
    menuToolButtonStyle->addSection(tr("Text Position"));
    menuToolButtonStyle->addActions(m_actionsToolButtonStyle->actions());

    auto *menuSettings = menuBar()->addMenu(tr("&Settings"));
    menuSettings->setObjectName(QStringLiteral("menuSettings"));
    menuSettings->addAction(m_actionShowPath);
    menuSettings->addSeparator();
    menuSettings->addAction(m_actionShowMenubar);
    menuSettings->addSeparator();
    menuSettings->addAction(m_actionShowToolbarApplication);
    menuSettings->addAction(m_actionShowToolbarDocument);
    menuSettings->addAction(m_actionShowToolbarEdit);
    menuSettings->addAction(m_actionShowToolbarView);
    menuSettings->addAction(m_actionShowToolbarFormat);
    menuSettings->addAction(m_actionShowToolbarTools);
    menuSettings->addAction(m_actionShowToolbarSettings);
    menuSettings->addAction(m_actionShowToolbarHelp);
    menuSettings->addMenu(menuToolButtonStyle);
    menuSettings->addSeparator();
    menuSettings->addAction(m_actionShowStatusbar);
    menuSettings->addSeparator();
    menuSettings->addAction(m_actionFullScreen);

    m_toolbarSettings = addToolBar(tr("Settings Toolbar"));
    m_toolbarSettings->setObjectName(QStringLiteral("toolbarSettings"));
    m_toolbarSettings->addAction(m_actionShowMenubar);
    m_toolbarSettings->addAction(m_actionShowStatusbar);
    m_toolbarSettings->addSeparator();
    m_toolbarSettings->addAction(m_actionFullScreen);


    //
    // Help

    auto *menuHelp = menuBar()->addMenu(tr("&Help"));
    menuHelp->setObjectName(QStringLiteral("menuHelp"));

    m_toolbarHelp = addToolBar(tr("Help Toolbar"));
    m_toolbarHelp->setObjectName(QStringLiteral("toolbarHelp"));


    // Connect toolbars with the corresponding actions
    connect(m_toolbarApplication, &QToolBar::visibilityChanged, m_actionShowToolbarApplication, &QAction::setChecked);
    connect(m_actionShowToolbarApplication, &QAction::toggled, m_toolbarApplication, &QToolBar::setVisible);
    connect(m_toolbarDocument, &QToolBar::visibilityChanged, m_actionShowToolbarDocument, &QAction::setChecked);
    connect(m_actionShowToolbarDocument, &QAction::toggled, m_toolbarDocument, &QToolBar::setVisible);
    connect(m_toolbarEdit, &QToolBar::visibilityChanged, m_actionShowToolbarEdit, &QAction::setChecked);
    connect(m_actionShowToolbarEdit, &QAction::toggled, m_toolbarEdit, &QToolBar::setVisible);
    connect(m_toolbarView, &QToolBar::visibilityChanged, m_actionShowToolbarView, &QAction::setChecked);
    connect(m_actionShowToolbarView, &QAction::toggled, m_toolbarView, &QToolBar::setVisible);
    connect(m_toolbarFormat, &QToolBar::visibilityChanged, m_actionShowToolbarFormat, &QAction::setChecked);
    connect(m_actionShowToolbarFormat, &QAction::toggled, m_toolbarFormat, &QToolBar::setVisible);
    connect(m_toolbarTools, &QToolBar::visibilityChanged, m_actionShowToolbarTools, &QAction::setChecked);
    connect(m_actionShowToolbarTools, &QAction::toggled, m_toolbarTools, &QToolBar::setVisible);
    connect(m_toolbarSettings, &QToolBar::visibilityChanged, m_actionShowToolbarSettings, &QAction::setChecked);
    connect(m_actionShowToolbarSettings, &QAction::toggled, m_toolbarSettings, &QToolBar::setVisible);
    connect(m_toolbarHelp, &QToolBar::visibilityChanged, m_actionShowToolbarHelp, &QAction::setChecked);
    connect(m_actionShowToolbarHelp, &QAction::toggled, m_toolbarHelp, &QToolBar::setVisible);


    //
    // Statusbar

    statusBar()->showMessage(tr("Ready"), 3000);
}


void MainWindow::updateActionsToolButtonStyle(const Qt::ToolButtonStyle style)
{
    const QList<QAction *> actions = m_actionsToolButtonStyle->actions();
    for (auto *action : actions) {
        if (static_cast<Qt::ToolButtonStyle>(action->data().toInt()) == style) {
            action->trigger();
            break;
        }
    }
}


void MainWindow::updateActionFullScreen()
{
    if (!m_actionFullScreen->isChecked()) {
        m_actionFullScreen->setText(tr("Full S&creen Mode"));
        m_actionFullScreen->setIcon(QIcon::fromTheme(QStringLiteral("view-fullscreen"), QIcon(QStringLiteral(":/icons/actions/16/view-fullscreen.svg"))));
        m_actionFullScreen->setIconText(tr("Full Screen"));
        m_actionFullScreen->setToolTip(tr("Display the window in full screen"));
    }
    else {
        m_actionFullScreen->setText(tr("Exit Full S&creen Mode"));
        m_actionFullScreen->setIcon(QIcon::fromTheme(QStringLiteral("view-restore"), QIcon(QStringLiteral(":/icons/actions/16/view-restore.svg"))));
        m_actionFullScreen->setIconText(tr("Full Screen"));
        m_actionFullScreen->setToolTip(tr("Exit full screen mode"));
    }
}


void MainWindow::enableActions(const bool enabled)
{
    m_actionSave->setEnabled(enabled);
    m_actionSaveAs->setEnabled(enabled);
    m_actionSaveCopyAs->setEnabled(enabled);
    m_actionSaveAll->setEnabled(enabled);
    m_actionClose->setEnabled(enabled);
    m_actionCloseAll->setEnabled(enabled);
}


void MainWindow::enableActionCloseOther(const bool enabled)
{
    m_actionCloseOther->setEnabled(enabled);
}


void MainWindow::enableUrlActions(const bool enabled)
{
    m_actionCopyPath->setEnabled(enabled);
}


void MainWindow::enableFileActions(const bool enabled)
{
    m_actionRenameFilename->setEnabled(enabled);
}


void MainWindow::loadSettings()
{
    QSettings settings;


    //
    // Application properties

    // Geometry
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

    // State
    const auto state = settings.value(QStringLiteral("Application/State"), QByteArray()).toByteArray();
    if (!state.isEmpty()) {
        restoreState(state);

        if (isFullScreen())
            m_actionFullScreen->toggle();
    }
    else {
        // Default: Show/hide toolbars
        m_toolbarApplication->setVisible(true);
        m_toolbarDocument->setVisible(true);
        m_toolbarEdit->setVisible(true);
        m_toolbarView->setVisible(true);
        m_toolbarFormat->setVisible(true);
        m_toolbarTools->setVisible(true);
        m_toolbarSettings->setVisible(false);
        m_toolbarHelp->setVisible(false);
    }

    // Show Path
    bool visible = settings.value(QStringLiteral("Application/ShowPath"), true).toBool();
    if (!visible)
        m_actionShowPath->toggle();

    // Show Menubar
    visible = settings.value(QStringLiteral("Application/ShowMenubar"), true).toBool();
    if (!visible)
        m_actionShowMenubar->toggle();

    // Show Statusbar
    visible = settings.value(QStringLiteral("Application/ShowStatusbar"), true).toBool();
    if (!visible)
        m_actionShowStatusbar->toggle();

    // Tool Button Style
    const auto value = settings.value(QStringLiteral("Application/ToolButtonStyle"), static_cast<int>(Qt::ToolButtonFollowStyle)).toInt();
    const QMetaEnum styles(QMetaEnum::fromType<Qt::ToolButtonStyle>());
    const auto style = !!styles.valueToKey(value) ? static_cast<Qt::ToolButtonStyle>(value) : Qt::ToolButtonFollowStyle;
    updateActionsToolButtonStyle(style);
}


void MainWindow::saveSettings()
{
    QSettings settings;


    //
    // Application properties

    const auto geometry = saveGeometry();
    settings.setValue(QStringLiteral("Application/Geometry"), geometry);

    const auto state = saveState();
    settings.setValue(QStringLiteral("Application/State"), state);

    bool visible = m_actionShowPath->isChecked();
    settings.setValue(QStringLiteral("Application/ShowPath"), visible);

    visible = m_actionShowMenubar->isChecked();
    settings.setValue(QStringLiteral("Application/ShowMenubar"), visible);

    visible = m_actionShowStatusbar->isChecked();
    settings.setValue(QStringLiteral("Application/ShowStatusbar"), visible);

    const int value = m_actionsToolButtonStyle->checkedAction()->data().toInt();
    settings.setValue(QStringLiteral("Application/ToolButtonStyle"), value);
}


void MainWindow::closeEvent(QCloseEvent *event)
{
    if (m_documentsArea->subWindowCount() >= 1) {

        const QString &title = tr("Quit the application");
        const QString &text = tr("This will close all open documents and quit the application.\n"
                                 "Are you sure you want to continue?");
        const QMessageBox::StandardButtons buttons = QMessageBox::Yes | QMessageBox::Cancel;
        const QMessageBox::StandardButton defaultButton = QMessageBox::Yes;

        if (QMessageBox::warning(this, title, text, buttons, defaultButton) == QMessageBox::Cancel) {
            event->ignore();
            return;
        }

        m_documentsArea->closeAllSubWindows();
    }

    saveSettings();
    event->accept();
}


//
// Application window
//

void MainWindow::updateWindowModified()
{
    bool modified = false;

    auto *docWindow = qobject_cast<MdiWindow *>(m_documentsArea->activeSubWindow());
    if (docWindow)
        modified = docWindow->isWindowModified();

    setWindowModified(modified);
}


void MainWindow::updateWindowTitle()
{
    QString caption;

    auto *docWindow = qobject_cast<MdiWindow *>(m_documentsArea->activeSubWindow());
    if (docWindow) {
        const bool pathVisible = m_actionShowPath->isChecked();
        caption = tr("%1 [*]").arg(docWindow->windowCaption(pathVisible));
    }

    setWindowTitle(caption);
}


//
// Document
//

DocumentWidget *MainWindow::createDocument()
{
    auto *document = new DocumentWidget;
    auto *docWindow = new MdiWindow;
    docWindow->setWidget(document);
    m_documentsArea->addSubWindow(docWindow);

    // Connections: Modified
    connect(document, &DocumentWidget::modifiedChanged, docWindow, &MdiWindow::documentModifiedChanged);
    connect(document, &DocumentWidget::modifiedChanged, this, &MainWindow::documentModifiedChanged);
    // Connections: Url
    connect(document, &DocumentWidget::urlChanged, docWindow, &MdiWindow::documentUrlChanged);
    connect(document, &DocumentWidget::urlChanged, this, &MainWindow::documentUrlChanged);
    // Connections: Actions
    connect(docWindow, &MdiWindow::actionCloseOther, m_documentsArea, &MdiArea::closeOtherSubWindows);
    connect(docWindow, &MdiWindow::actionCopyPath, document, &DocumentWidget::copyPathToClipboard);
    connect(docWindow, &MdiWindow::actionRenameFilename, document, &DocumentWidget::renameFilename);
    // Connections: Document count
    connect(docWindow, &MdiWindow::destroyed, this, &MainWindow::documentClosed);
    connect(this, &MainWindow::documentCountChanged, document, &DocumentWidget::documentCountChanged);
    connect(this, &MainWindow::documentCountChanged, docWindow, &MdiWindow::documentCountChanged);

    // Initialize
    document->initModified();
    document->initUrl();

    return document;
}


bool MainWindow::openDocument(const QUrl &url)
{
    if (!url.isValid())
        return false;

    QMdiSubWindow *subWindow = m_documentsArea->findSubWindow(url);
    if (subWindow) {
        // Given document is already loaded; activate the subwindow
        m_documentsArea->setActiveSubWindow(subWindow);
        return true;
    }

    return loadDocument(url);
}


bool MainWindow::loadDocument(const QUrl &url)
{
    DocumentWidget *document = createDocument();
    if (!true) {
        // Given document could not be loaded
        document->close();
        return false;
    }

    document->show();
    document->setUrl(url);

    documentCreated();

    return true;
}


bool MainWindow::saveDocument(DocumentWidget *document, const QUrl &altUrl)
{
    Q_UNUSED(altUrl)

    document->resetModified();

    return true;
}


//
//
//

void MainWindow::documentCreated()
{
    const int count = m_documentsArea->subWindowCount();

    emit documentCountChanged(count);
    enableActionCloseOther(count >= 2);
}


void MainWindow::documentActivated(QMdiSubWindow *subWindow)
{
    updateWindowModified();
    updateWindowTitle();

    DocumentWidget *document = extractDocument(subWindow);
    if (document) {

    }
    else {

    }

    enableActions(hasActiveDocument());
    enableUrlActions(hasActiveDocumentUrl());
    enableFileActions(hasActiveDocumentUrlFile());
}


void MainWindow::documentModifiedChanged(const bool modified)
{
    Q_UNUSED(modified)

    if (sender() == activeDocument())
        updateWindowModified();
}


void MainWindow::documentUrlChanged(const QUrl &url)
{
    Q_UNUSED(url)

    if (sender() == activeDocument()) {
        updateWindowTitle();

        enableUrlActions(hasActiveDocumentUrl());
        enableFileActions(hasActiveDocumentUrlFile());
    }
}


void MainWindow::documentClosed()
{
    const int count = m_documentsArea->subWindowCount();

    emit documentCountChanged(count);
    enableActionCloseOther(count >= 2);
}


//
//
//

DocumentWidget *MainWindow::extractDocument(const QMdiSubWindow *subWindow) const
{
    if (!subWindow)
        return nullptr;

    return qobject_cast<DocumentWidget *>(subWindow->widget());
}


DocumentWidget *MainWindow::activeDocument() const
{
    return extractDocument(m_documentsArea->activeSubWindow());
}


bool MainWindow::hasActiveDocument() const
{
    return !!activeDocument();
}


bool MainWindow::hasActiveDocumentUrl() const
{
    auto *document = activeDocument();
    if (!document)
        return false;

    return !document->url().isEmpty();
}


bool MainWindow::hasActiveDocumentUrlFile() const
{
    auto *document = activeDocument();
    if (!document)
        return false;

    return document->url().isLocalFile();
}


//
// Action slots
//

void MainWindow::slotAbout()
{
    auto *dialog = new AboutDialog(this);
    dialog->open();
}


void MainWindow::slotColophon()
{
    auto *dialog = new ColophonDialog(this);
    dialog->open();
}


void MainWindow::slotPreferences()
{
    auto *dialog = new PreferencesDialog(this);
    dialog->open();
}


void MainWindow::slotNew()
{
    DocumentWidget *document = createDocument();
    document->show();

    documentCreated();
}


void MainWindow::slotOpen()
{
    const QList<QUrl> urls = QFileDialog::getOpenFileUrls(this, tr("Open Documents"));
    for (const QUrl &url : urls)
        openDocument(url);
}


void MainWindow::slotSave()
{
    DocumentWidget *document = activeDocument();
    if (!document)
        return;

    if (!document->url().isEmpty())
        saveDocument(document, QUrl());
    else
        slotSaveAs();
}


void MainWindow::slotSaveAs()
{
    DocumentWidget *document = activeDocument();
    if (!document)
        return;

    const QUrl &url = QFileDialog::getSaveFileUrl(this, tr("Save Document"));
    if (!url.isEmpty()) {
        document->setUrl(url);
        saveDocument(document, QUrl());
    }
}


void MainWindow::slotSaveCopyAs()
{
    DocumentWidget *document = activeDocument();
    if (!document)
        return;

    const QUrl &url = QFileDialog::getSaveFileUrl(this, tr("Save Copy of Document"));
    if (!url.isEmpty())
        saveDocument(document, url);
}


void MainWindow::slotSaveAll()
{
    const QList<QMdiSubWindow *> subWindows = m_documentsArea->subWindowList();
    for (auto *subWindow : subWindows) {

        DocumentWidget *document = extractDocument(subWindow);
        if (!document)
            continue;

        if (!document->url().isEmpty()) {
            saveDocument(document, QUrl());
        }
        else {
            const QUrl &url = QFileDialog::getSaveFileUrl(this, tr("Save Document"));
            if (!url.isEmpty()) {
                document->setUrl(url);
                saveDocument(document, QUrl());
            }
        }
    }
}


void MainWindow::slotCopyPath()
{
    DocumentWidget *document = activeDocument();
    if (!document)
        return;

    document->copyPathToClipboard();
}


void MainWindow::slotRenameFilename()
{
    DocumentWidget *document = activeDocument();
    if (!document)
        return;

    document->renameFilename();
}


void MainWindow::slotCloseOther()
{
    if (m_documentsArea->subWindowCount() >= 2) {

        const QString &title = tr("Close all documents beside current one");
        const QString &text = tr("This will close all open documents beside the current one.\n"
                                 "Are you sure you want to continue?");
        const QMessageBox::StandardButtons buttons = QMessageBox::Yes | QMessageBox::Cancel;
        const QMessageBox::StandardButton defaultButton = QMessageBox::Yes;

        if (QMessageBox::warning(this, title, text, buttons, defaultButton) != QMessageBox::Cancel)
            m_documentsArea->closeOtherSubWindows(m_documentsArea->activeSubWindow());
    }
}


void MainWindow::slotCloseAll()
{
    if (m_documentsArea->subWindowCount() >= 1) {

        const QString &title = tr("Close all documents");
        const QString &text = tr("This will close all open documents.\n"
                                 "Are you sure you want to continue?");
        const QMessageBox::StandardButtons buttons = QMessageBox::Yes | QMessageBox::Cancel;
        const QMessageBox::StandardButton defaultButton = QMessageBox::Yes;

        if (QMessageBox::warning(this, title, text, buttons, defaultButton) != QMessageBox::Cancel)
            m_documentsArea->closeAllSubWindows();
    }
}


void MainWindow::slotToolButtonStyle(const QAction *action)
{
    const auto style = static_cast<Qt::ToolButtonStyle>(action->data().toInt());

    m_toolbarApplication->setToolButtonStyle(style);
    m_toolbarDocument->setToolButtonStyle(style);
    m_toolbarEdit->setToolButtonStyle(style);
    m_toolbarView->setToolButtonStyle(style);
    m_toolbarFormat->setToolButtonStyle(style);
    m_toolbarTools->setToolButtonStyle(style);
    m_toolbarSettings->setToolButtonStyle(style);
    m_toolbarHelp->setToolButtonStyle(style);
}


void MainWindow::slotFullScreen(const bool checked)
{
    updateActionFullScreen();

    if (checked)
        setWindowState(windowState() | Qt::WindowFullScreen);
    else
        setWindowState(windowState() & ~Qt::WindowFullScreen);
}
