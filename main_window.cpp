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
#include <QScreen>
#include <QSettings>
#include <QStatusBar>
#include <QToolBar>
#include <QUrl>

#include "about_dialog.h"
#include "colophon_dialog.h"
#include "mdi_area.h"
#include "mdi_document.h"
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
    connect(m_documentsArea, &MdiArea::subWindowActivated, this, &MainWindow::activateDocument);
    setCentralWidget(m_documentsArea);

    setupActions();

    loadSettings();

    m_actionViewFullScreen->setChecked(isFullScreen());
    updateActionViewFullScreen();

    enableActions();
}

MainWindow::~MainWindow()
{

}


void MainWindow::closeEvent(QCloseEvent *event)
{
    if (m_documentsArea->subWindowCount() > 0) {

        QString title = tr("Quit the application");
        QString text = tr("This will close all open documents and quit the application.\n"
                          "Are you sure you want to continue?");

        if (QMessageBox::warning(this, title, text, QMessageBox::Yes | QMessageBox::Cancel, QMessageBox::Yes) != QMessageBox::Cancel) {

            m_documentsArea->closeAllSubWindows();
            saveSettings();

            event->accept();
            return;
        }
    }
    else if (m_documentsArea->subWindowCount() == 0) {

        saveSettings();

        event->accept();
        return;
    }

    event->ignore();
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
    connect(m_actionAbout, &QAction::triggered, this, &MainWindow::onActionAboutTriggered);

    m_actionColophon = new QAction(tr("&Colophon"), this);
    m_actionColophon->setObjectName(QStringLiteral("actionColophon"));
    m_actionColophon->setIcon(QIcon::fromTheme(QStringLiteral("help-about"), QIcon(QStringLiteral(":/icons/actions/16/help-about.svg"))));
    m_actionColophon->setToolTip(tr("Lengthy description of the application"));
    m_actionColophon->setMenuRole(QAction::ApplicationSpecificRole);
    connect(m_actionColophon, &QAction::triggered, this, &MainWindow::onActionColophonTriggered);

    m_actionPreferences = new QAction(tr("&Preferences…"), this);
    m_actionPreferences->setObjectName(QStringLiteral("actionPreferences"));
    m_actionPreferences->setIcon(QIcon::fromTheme(QStringLiteral("configure"), QIcon(QStringLiteral(":/icons/actions/16/configure.svg"))));
    m_actionPreferences->setToolTip(tr("Customize the appearance and behavior of the application"));
    m_actionPreferences->setMenuRole(QAction::PreferencesRole);
    connect(m_actionPreferences, &QAction::triggered, this, &MainWindow::onActionPreferencesTriggered);

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
    m_toolbarApplication->addAction(m_actionColophon);
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
    connect(m_actionNew, &QAction::triggered, this, &MainWindow::onActionNewTriggered);
    addAction(m_actionNew);

    m_actionOpen = new QAction(tr("&Open…"), this);
    m_actionOpen->setObjectName(QStringLiteral("actionOpen"));
    m_actionOpen->setIcon(QIcon::fromTheme(QStringLiteral("document-open"), QIcon(QStringLiteral(":/icons/actions/16/document-open.svg"))));
    m_actionOpen->setShortcut(QKeySequence::Open);
    m_actionOpen->setToolTip(tr("Open an existing document"));
    connect(m_actionOpen, &QAction::triggered, this, &MainWindow::onActionOpenTriggered);
    addAction(m_actionOpen);

    m_actionSave = new QAction(tr("&Save"), this);
    m_actionSave->setObjectName(QStringLiteral("actionSave"));
    m_actionSave->setIcon(QIcon::fromTheme(QStringLiteral("document-save"), QIcon(QStringLiteral(":/icons/actions/16/document-save.svg"))));
    m_actionSave->setShortcut(QKeySequence::Save);
    m_actionSave->setToolTip(tr("Save document"));
    connect(this, &MainWindow::actionsIsEnabled, m_actionSave, &QAction::setEnabled);
    connect(m_actionSave, &QAction::triggered, this, &MainWindow::onActionSaveTriggered);
    addAction(m_actionSave);

    m_actionSaveAs = new QAction(tr("Save &As…"), this);
    m_actionSaveAs->setObjectName(QStringLiteral("actionSaveAs"));
    m_actionSaveAs->setIcon(QIcon::fromTheme(QStringLiteral("document-save-as"), QIcon(QStringLiteral(":/icons/actions/16/document-save-as.svg"))));
    m_actionSaveAs->setShortcut(QKeySequence::SaveAs);
    m_actionSaveAs->setToolTip(tr("Save document under a new name"));
    connect(this, &MainWindow::actionsIsEnabled, m_actionSaveAs, &QAction::setEnabled);
    connect(m_actionSaveAs, &QAction::triggered, this, &MainWindow::onActionSaveAsTriggered);
    addAction(m_actionSaveAs);

    m_actionSaveCopyAs = new QAction(tr("Save &Copy As…"), this);
    m_actionSaveCopyAs->setObjectName(QStringLiteral("actionSaveCopyAs"));
    m_actionSaveCopyAs->setIcon(QIcon::fromTheme(QStringLiteral("document-save-as"), QIcon(QStringLiteral(":/icons/actions/16/document-save-as.svg"))));
    m_actionSaveCopyAs->setToolTip(tr("Save copy of document under a new name"));
    connect(this, &MainWindow::actionsIsEnabled, m_actionSaveCopyAs, &QAction::setEnabled);
    connect(m_actionSaveCopyAs, &QAction::triggered, this, &MainWindow::onActionSaveCopyAsTriggered);

    m_actionSaveAll = new QAction(tr("Save A&ll"), this);
    m_actionSaveAll->setObjectName(QStringLiteral("actionSaveAll"));
    m_actionSaveAll->setIcon(QIcon::fromTheme(QStringLiteral("document-save-all"), QIcon(QStringLiteral(":/icons/actions/16/document-save-all.svg"))));
    m_actionSaveAll->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_L));
    m_actionSaveAll->setToolTip(tr("Save all open documents"));
    connect(this, &MainWindow::actionsIsEnabled, m_actionSaveAll, &QAction::setEnabled);
    connect(m_actionSaveAll, &QAction::triggered, this, &MainWindow::onActionSaveAllTriggered);
    addAction(m_actionSaveAll);

    m_actionCopyPath = new QAction(tr("Cop&y Path"), this);
    m_actionCopyPath->setObjectName(QStringLiteral("actionCopyPath"));
    m_actionCopyPath->setIcon(QIcon::fromTheme(QStringLiteral("edit-copy"), QIcon(QStringLiteral(":/icons/actions/16/edit-copy.svg"))));
    m_actionCopyPath->setToolTip(tr("Copy document path to clipboard"));
    connect(this, &MainWindow::actionsFileIsEnabled, m_actionCopyPath, &QAction::setEnabled);
    connect(m_actionCopyPath, &QAction::triggered, this, &MainWindow::onActionCopyPathTriggered);

    m_actionRename = new QAction(tr("Re&name…"), this);
    m_actionRename->setObjectName(QStringLiteral("actionRename"));
    m_actionRename->setIcon(QIcon::fromTheme(QStringLiteral("edit-rename"), QIcon(QStringLiteral(":/icons/actions/16/edit-rename.svg"))));
    m_actionRename->setToolTip(tr("Rename file name of the document"));
    connect(this, &MainWindow::actionsFileIsEnabled, m_actionRename, &QAction::setEnabled);
    connect(m_actionRename, &QAction::triggered, this, &MainWindow::onActionRenameTriggered);

    m_actionClose = new QAction(tr("&Close"), this);
    m_actionClose->setObjectName(QStringLiteral("actionClose"));
    m_actionClose->setIcon(QIcon::fromTheme(QStringLiteral("document-close"), QIcon(QStringLiteral(":/icons/actions/16/document-close.svg"))));
    m_actionClose->setShortcut(QKeySequence::Close);
    m_actionClose->setToolTip(tr("Close document"));
    connect(this, &MainWindow::actionsIsEnabled, m_actionClose, &QAction::setEnabled);
    connect(m_actionClose, &QAction::triggered, m_documentsArea, &MdiArea::closeActiveSubWindow);
    addAction(m_actionClose);

    m_actionCloseOther = new QAction(tr("Close Ot&her"), this);
    m_actionCloseOther->setObjectName(QStringLiteral("actionCloseOther"));
    m_actionCloseOther->setToolTip(tr("Close other open documents"));
    connect(this, &MainWindow::actionCloseOtherIsEnabled, m_actionCloseOther, &QAction::setEnabled);
    connect(m_actionCloseOther, &QAction::triggered, this, [=] () { onActionCloseOtherTriggered(); });

    m_actionCloseAll = new QAction(tr("Clos&e All"), this);
    m_actionCloseAll->setObjectName(QStringLiteral("actionCloseAll"));
    m_actionCloseAll->setToolTip(tr("Close all open documents"));
    connect(this, &MainWindow::actionsIsEnabled, m_actionCloseAll, &QAction::setEnabled);
    connect(m_actionCloseAll, &QAction::triggered, this, &MainWindow::onActionCloseAllTriggered);

    auto *menuFile = menuBar()->addMenu(tr("&File"));
    menuFile->setObjectName(QStringLiteral("menuFile"));
    menuFile->addAction(m_actionNew);
    menuFile->addSeparator();
    menuFile->addAction(m_actionOpen);
    menuFile->addSeparator();
    menuFile->addAction(m_actionSave);
    menuFile->addAction(m_actionSaveAs);
    menuFile->addAction(m_actionSaveCopyAs);
    menuFile->addAction(m_actionSaveAll);
    menuFile->addSeparator();
    menuFile->addAction(m_actionCopyPath);
    menuFile->addSeparator();
    menuFile->addAction(m_actionRename);
    menuFile->addSeparator();
    menuFile->addAction(m_actionClose);
    menuFile->addAction(m_actionCloseOther);
    menuFile->addAction(m_actionCloseAll);

    m_toolbarFile = addToolBar(tr("File Toolbar"));
    m_toolbarFile->setObjectName(QStringLiteral("toolbarFile"));
    m_toolbarFile->addAction(m_actionNew);
    m_toolbarFile->addAction(m_actionOpen);
    m_toolbarFile->addSeparator();
    m_toolbarFile->addAction(m_actionSave);
    m_toolbarFile->addAction(m_actionSaveAs);
    m_toolbarFile->addSeparator();
    m_toolbarFile->addAction(m_actionClose);


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

    auto *menuFormat = menuBar()->addMenu(tr("F&ormat"));
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
    m_actionShowPath->setToolTip(tr("Show document path in the window caption"));
    connect(m_actionShowPath, &QAction::toggled, this, &MainWindow::updateWindowTitle);

    m_actionShowMenubar = new QAction(tr("Show &Menubar"), this);
    m_actionShowMenubar->setObjectName(QStringLiteral("actionShowMenubar"));
    m_actionShowMenubar->setCheckable(true);
    m_actionShowMenubar->setIcon(QIcon::fromTheme(QStringLiteral("show-menu"), QIcon(QStringLiteral(":/icons/actions/16/show-menu.svg"))));
    m_actionShowMenubar->setIconText(tr("Menubar"));
    m_actionShowMenubar->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_M));
    m_actionShowMenubar->setToolTip(tr("Show the menubar"));
    connect(m_actionShowMenubar, &QAction::toggled, menuBar(), &QMenuBar::setVisible);
    addAction(m_actionShowMenubar);

    m_actionShowToolbarApplication = new QAction(tr("Show &Application Toolbar"), this);
    m_actionShowToolbarApplication->setObjectName(QStringLiteral("actionShowToolbarApplication"));
    m_actionShowToolbarApplication->setCheckable(true);
    m_actionShowToolbarApplication->setToolTip(tr("Show the Application toolbar"));

    m_actionShowToolbarFile = new QAction(tr("Show &File Toolbar"), this);
    m_actionShowToolbarFile->setObjectName(QStringLiteral("actionShowToolbarFile"));
    m_actionShowToolbarFile->setCheckable(true);
    m_actionShowToolbarFile->setToolTip(tr("Show the File toolbar"));

    m_actionShowToolbarEdit = new QAction(tr("Show &Edit Toolbar"), this);
    m_actionShowToolbarEdit->setObjectName(QStringLiteral("actionShowToolbarEdit"));
    m_actionShowToolbarEdit->setCheckable(true);
    m_actionShowToolbarEdit->setToolTip(tr("Show the Edit toolbar"));

    m_actionShowToolbarView = new QAction(tr("Show &View Toolbar"), this);
    m_actionShowToolbarView->setObjectName(QStringLiteral("actionShowToolbarView"));
    m_actionShowToolbarView->setCheckable(true);
    m_actionShowToolbarView->setToolTip(tr("Show the View toolbar"));

    m_actionShowToolbarFormat = new QAction(tr("Show F&ormat Toolbar"), this);
    m_actionShowToolbarFormat->setObjectName(QStringLiteral("actionShowToolbarFormat"));
    m_actionShowToolbarFormat->setCheckable(true);
    m_actionShowToolbarFormat->setToolTip(tr("Show the Format toolbar"));

    m_actionShowToolbarTools = new QAction(tr("Show &Tools Toolbar"), this);
    m_actionShowToolbarTools->setObjectName(QStringLiteral("actionShowToolbarTools"));
    m_actionShowToolbarTools->setCheckable(true);
    m_actionShowToolbarTools->setToolTip(tr("Show the Tools toolbar"));

    m_actionShowToolbarAppearance = new QAction(tr("Show Appea&rance Toolbar"), this);
    m_actionShowToolbarAppearance->setObjectName(QStringLiteral("actionShowToolbarAppearance"));
    m_actionShowToolbarAppearance->setCheckable(true);
    m_actionShowToolbarAppearance->setToolTip(tr("Show the Appearance toolbar"));

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

    auto *actionToolButtonStyleFollowStyle = new QAction(tr("&Follow Style"), this);
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
    connect(m_actionsToolButtonStyle, &QActionGroup::triggered, this, &MainWindow::onActionsToolButtonStyleTriggered);

    m_actionShowStatusbar = new QAction(tr("Show Stat&usbar"), this);
    m_actionShowStatusbar->setObjectName(QStringLiteral("actionShowStatusbar"));
    m_actionShowStatusbar->setCheckable(true);
    m_actionShowStatusbar->setToolTip(tr("Show the Statusbar"));
    connect(m_actionShowStatusbar, &QAction::toggled, statusBar(), &QStatusBar::setVisible);

    m_actionViewFullScreen = new QAction(this);
    m_actionViewFullScreen->setObjectName(QStringLiteral("actionViewFullScreen"));
    m_actionViewFullScreen->setCheckable(true);
    m_actionViewFullScreen->setShortcuts(QList<QKeySequence>() << QKeySequence(Qt::Key_F11) << QKeySequence::FullScreen);
    connect(m_actionViewFullScreen, &QAction::toggled, this, &MainWindow::onActionViewFullScreenTriggered);
    addAction(m_actionViewFullScreen);

    auto *menuToolButtonStyle = new QMenu(tr("Tool Button St&yle"), this);
    menuToolButtonStyle->setObjectName(QStringLiteral("menuToolButtonStyle"));
    menuToolButtonStyle->addActions(m_actionsToolButtonStyle->actions());

    auto *menuAppearance = menuBar()->addMenu(tr("Appea&rance"));
    menuAppearance->setObjectName(QStringLiteral("menuAppearance"));
    menuAppearance->addAction(m_actionShowPath);
    menuAppearance->addSeparator();
    menuAppearance->addAction(m_actionShowMenubar);
    menuAppearance->addSeparator();
    menuAppearance->addAction(m_actionShowToolbarApplication);
    menuAppearance->addAction(m_actionShowToolbarFile);
    menuAppearance->addAction(m_actionShowToolbarEdit);
    menuAppearance->addAction(m_actionShowToolbarView);
    menuAppearance->addAction(m_actionShowToolbarFormat);
    menuAppearance->addAction(m_actionShowToolbarTools);
    menuAppearance->addAction(m_actionShowToolbarAppearance);
    menuAppearance->addAction(m_actionShowToolbarHelp);
    menuAppearance->addMenu(menuToolButtonStyle);
    menuAppearance->addSeparator();
    menuAppearance->addAction(m_actionShowStatusbar);
    menuAppearance->addSeparator();
    menuAppearance->addAction(m_actionViewFullScreen);

    m_toolbarAppearance = addToolBar(tr("Appearance Toolbar"));
    m_toolbarAppearance->setObjectName(QStringLiteral("toolbarAppearance"));
    m_toolbarAppearance->addAction(m_actionShowMenubar);
    m_toolbarAppearance->addSeparator();
    m_toolbarAppearance->addAction(m_actionViewFullScreen);


    //
    // Help

    auto *menuHelp = menuBar()->addMenu(tr("&Help"));
    menuHelp->setObjectName(QStringLiteral("menuHelp"));

    m_toolbarHelp = addToolBar(tr("Help Toolbar"));
    m_toolbarHelp->setObjectName(QStringLiteral("toolbarHelp"));


    // Connect toolbars with the corresponding actions
    connect(m_toolbarApplication, &QToolBar::visibilityChanged, m_actionShowToolbarApplication, &QAction::setChecked);
    connect(m_actionShowToolbarApplication, &QAction::toggled, m_toolbarApplication, &QToolBar::setVisible);
    connect(m_toolbarFile, &QToolBar::visibilityChanged, m_actionShowToolbarFile, &QAction::setChecked);
    connect(m_actionShowToolbarFile, &QAction::toggled, m_toolbarFile, &QToolBar::setVisible);
    connect(m_toolbarEdit, &QToolBar::visibilityChanged, m_actionShowToolbarEdit, &QAction::setChecked);
    connect(m_actionShowToolbarEdit, &QAction::toggled, m_toolbarEdit, &QToolBar::setVisible);
    connect(m_toolbarView, &QToolBar::visibilityChanged, m_actionShowToolbarView, &QAction::setChecked);
    connect(m_actionShowToolbarView, &QAction::toggled, m_toolbarView, &QToolBar::setVisible);
    connect(m_toolbarFormat, &QToolBar::visibilityChanged, m_actionShowToolbarFormat, &QAction::setChecked);
    connect(m_actionShowToolbarFormat, &QAction::toggled, m_toolbarFormat, &QToolBar::setVisible);
    connect(m_toolbarTools, &QToolBar::visibilityChanged, m_actionShowToolbarTools, &QAction::setChecked);
    connect(m_actionShowToolbarTools, &QAction::toggled, m_toolbarTools, &QToolBar::setVisible);
    connect(m_toolbarAppearance, &QToolBar::visibilityChanged, m_actionShowToolbarAppearance, &QAction::setChecked);
    connect(m_actionShowToolbarAppearance, &QAction::toggled, m_toolbarAppearance, &QToolBar::setVisible);
    connect(m_toolbarHelp, &QToolBar::visibilityChanged, m_actionShowToolbarHelp, &QAction::setChecked);
    connect(m_actionShowToolbarHelp, &QAction::toggled, m_toolbarHelp, &QToolBar::setVisible);


    //
    // Statusbar

    statusBar()->showMessage(tr("Ready"), 3000);
}


void MainWindow::updateActionsToolButtonStyle(const Qt::ToolButtonStyle toolButtonStyle)
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


void MainWindow::updateActionViewFullScreen()
{
    if (!m_actionViewFullScreen->isChecked()) {
        m_actionViewFullScreen->setText(tr("Full &Screen Mode"));
        m_actionViewFullScreen->setIcon(QIcon::fromTheme(QStringLiteral("view-fullscreen"), QIcon(QStringLiteral(":/icons/actions/16/view-fullscreen.svg"))));
        m_actionViewFullScreen->setIconText(tr("Full Screen"));
        m_actionViewFullScreen->setToolTip(tr("Display the window in full screen"));
    }
    else {
        m_actionViewFullScreen->setText(tr("Exit Full &Screen Mode"));
        m_actionViewFullScreen->setIcon(QIcon::fromTheme(QStringLiteral("view-restore"), QIcon(QStringLiteral(":/icons/actions/16/view-restore.svg"))));
        m_actionViewFullScreen->setIconText(tr("Full Screen"));
        m_actionViewFullScreen->setToolTip(tr("Exit full screen mode"));
    }
}


void MainWindow::enableActions(QMdiSubWindow *subWindow)
{
    emit actionsIsEnabled(!!subWindow);

    enableActionsFile();
    enableActionCloseOther();
}


void MainWindow::enableActionsFile()
{
    MdiDocument *document = activeDocument();

    emit actionsFileIsEnabled(document && !document->documentUrl().isEmpty());
}


void MainWindow::enableActionCloseOther()
{
    emit actionCloseOtherIsEnabled(m_documentsArea->subWindowCount() > 1);
}


void MainWindow::loadSettings()
{
    QSettings settings;


    //
    // Application properties

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

    bool visible = settings.value(QStringLiteral("Application/ShowPath"), true).toBool();
    m_actionShowPath->setChecked(visible);

    visible = settings.value(QStringLiteral("Application/ShowMenubar"), true).toBool();
    menuBar()->setVisible(visible);
    m_actionShowMenubar->setChecked(visible);

    visible = settings.value(QStringLiteral("Application/ShowStatusbar"), true).toBool();
    statusBar()->setVisible(visible);
    m_actionShowStatusbar->setChecked(visible);

    const auto style = settings.value(QStringLiteral("Application/ToolButtonStyle"), static_cast<int>(Qt::ToolButtonFollowStyle)).toInt();
    updateActionsToolButtonStyle(static_cast<Qt::ToolButtonStyle>(style));
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

    visible = menuBar()->isVisible();
    settings.setValue(QStringLiteral("Application/ShowMenubar"), visible);

    visible = statusBar()->isVisible();
    settings.setValue(QStringLiteral("Application/ShowStatusbar"), visible);

    const auto style = m_actionsToolButtonStyle->checkedAction()->data();
    settings.setValue(QStringLiteral("Application/ToolButtonStyle"), style);
}


MdiDocument *MainWindow::createDocument()
{
    auto *document = new MdiDocument;
    auto *docWindow = new MdiWindow;
    docWindow->setWidget(document);
    m_documentsArea->addSubWindow(docWindow);

    connect(document, &MdiDocument::documentUrlChanged, this, &MainWindow::updateWindowTitle);
    connect(document, &MdiDocument::documentUrlChanged, docWindow, &MdiWindow::documentUrlChanged);
    connect(document, &MdiDocument::documentUrlChanged, this, &MainWindow::enableActionsFile);
    connect(document, &MdiDocument::modifiedChanged, this, &MainWindow::setWindowModified);
    connect(document, &MdiDocument::modifiedChanged, docWindow, &MdiWindow::setWindowModified);
    connect(document, &MdiDocument::modifiedChanged, docWindow, &MdiWindow::updateWindowIcon);
    connect(docWindow, &MdiWindow::actionCloseOtherTriggered, this, &MainWindow::onActionCloseOtherTriggered);
    connect(docWindow, &MdiWindow::actionCopyPathTriggered, document, &MdiDocument::copyDocumentUrlToClipboard);
    connect(docWindow, &MdiWindow::destroyed, this, &MainWindow::enableActionCloseOther);
    connect(this, &MainWindow::actionCloseOtherIsEnabled, docWindow, &MdiWindow::actionCloseOtherIsEnabled);

    document->resetDocumentUrl();
    document->resetModified();

    return document;
}


MdiDocument *MainWindow::extractDocument(const QMdiSubWindow *subWindow) const
{
    if (subWindow)
        return qobject_cast<MdiDocument *>(subWindow->widget());

    return nullptr;
}


MdiDocument *MainWindow::activeDocument() const
{
    return extractDocument(m_documentsArea->activeSubWindow());
}


bool MainWindow::openDocument(const QUrl &url)
{
    if (QMdiSubWindow *subWindow = m_documentsArea->findSubWindow(url)) {
        // Given document is already loaded; activate the subwindow
        m_documentsArea->setActiveSubWindow(subWindow);
        return true;
    }

    return loadDocument(url);
}


bool MainWindow::loadDocument(const QUrl &url)
{
    MdiDocument *document = createDocument();

    if (!true) {
        // Given document could not be loaded
        document->close();
        return false;
    }

    document->setDocumentUrl(url);
    document->show();

    return true;
}


bool MainWindow::saveDocument(MdiDocument *document, const QUrl &url)
{
    Q_UNUSED(url)

    document->setModified(false);

    return true;
}


void MainWindow::activateDocument(QMdiSubWindow *subWindow)
{
    updateWindowTitle();
    enableActions(subWindow);
}


void MainWindow::updateWindowTitle()
{
    auto *docWindow = qobject_cast<MdiWindow *>(m_documentsArea->activeSubWindow());
    if (docWindow) {
        setWindowTitle(docWindow->windowCaption(m_actionShowPath->isChecked()) + QStringLiteral(" [*]"));
        setWindowModified(docWindow->isWindowModified());
    }
    else {
        setWindowTitle(QString());
        setWindowModified(false);
    }
}


void MainWindow::onActionAboutTriggered()
{
    auto *dialog = new AboutDialog(this);
    dialog->open();
}


void MainWindow::onActionColophonTriggered()
{
    auto *dialog = new ColophonDialog(this);
    dialog->open();
}


void MainWindow::onActionPreferencesTriggered()
{
    auto *dialog = new PreferencesDialog(this);
    dialog->open();
}


void MainWindow::onActionNewTriggered()
{
    MdiDocument *document = createDocument();
    document->show();
}


void MainWindow::onActionOpenTriggered()
{
    const QList<QUrl> urls = QFileDialog::getOpenFileUrls(this, tr("Open Document"));
    for (const QUrl &url : urls)
        openDocument(url);
}


void MainWindow::onActionSaveTriggered()
{
    MdiDocument *document = activeDocument();
    if (!document)
        return;

    if (!document->documentUrl().isEmpty())
        saveDocument(document, document->documentUrl());
    else
        onActionSaveAsTriggered();
}


void MainWindow::onActionSaveAsTriggered()
{
    MdiDocument *document = activeDocument();
    if (!document)
        return;

    const QUrl url = QFileDialog::getSaveFileUrl(this, tr("Save Document"));
    if (!url.isEmpty()) {
        document->setDocumentUrl(url);
        saveDocument(document, url);
    }
}


void MainWindow::onActionSaveCopyAsTriggered()
{
    MdiDocument *document = activeDocument();
    if (!document)
        return;

    const QUrl url = QFileDialog::getSaveFileUrl(this, tr("Save Copy of Document"));
    if (!url.isEmpty())
        saveDocument(document, url);
}


void MainWindow::onActionSaveAllTriggered()
{
    const QList<QMdiSubWindow *> subWindows = m_documentsArea->subWindowList();
    for (auto *subWindow : subWindows) {

        MdiDocument *document = extractDocument(subWindow);
        if (!document)
            continue;

        if (!document->documentUrl().isEmpty()) {
            saveDocument(document, document->documentUrl());
        }
        else {
            const QUrl url = QFileDialog::getSaveFileUrl(this, tr("Save Document"));
            if (!url.isEmpty()) {
                document->setDocumentUrl(url);
                saveDocument(document, url);
            }
        }
    }
}


void MainWindow::onActionCopyPathTriggered()
{
    MdiDocument *document = activeDocument();
    document->copyDocumentUrlToClipboard();
}


void MainWindow::onActionRenameTriggered()
{
    MdiDocument *document = activeDocument();
    if (document)
        document->renameDocumentFilename();
}


void MainWindow::onActionCloseOtherTriggered(QMdiSubWindow *subWindow)
{
    if (m_documentsArea->subWindowCount() > 1) {

        QString title;
        QString text;

        if (!subWindow) {
            // Current document
            subWindow = m_documentsArea->activeSubWindow();
            title = tr("Close all documents beside current one");
            text = tr("This will close all open documents beside the current one.\n"
                      "Are you sure you want to continue?");
        }
        else {
            title = tr("Close all documents beside selected one");
            text = tr("This will close all open documents beside the selected one.\n"
                      "Are you sure you want to continue?");
        }

        if (QMessageBox::warning(this, title, text, QMessageBox::Yes | QMessageBox::Cancel, QMessageBox::Yes) != QMessageBox::Cancel)
            m_documentsArea->closeOtherSubWindows(subWindow);
    }
}


void MainWindow::onActionCloseAllTriggered()
{
    if (m_documentsArea->subWindowCount() > 0) {

        QString title = tr("Close all documents");
        QString text = tr("This will close all open documents.\n"
                          "Are you sure you want to continue?");

        if (QMessageBox::warning(this, title, text, QMessageBox::Yes | QMessageBox::Cancel, QMessageBox::Yes) != QMessageBox::Cancel)
            m_documentsArea->closeAllSubWindows();
    }
}


void MainWindow::onActionsToolButtonStyleTriggered(const QAction *actionToolButtonStyle)
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


void MainWindow::onActionViewFullScreenTriggered(const bool checked)
{
    if (checked)
        setWindowState(windowState() | Qt::WindowFullScreen);
    else
        setWindowState(windowState() & ~Qt::WindowFullScreen);

    updateActionViewFullScreen();
}
