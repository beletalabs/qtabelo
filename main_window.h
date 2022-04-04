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

#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>

class QAction;
class QActionGroup;
class QCloseEvent;
class QMdiSubWindow;
class QStatusBar;
class QToolBar;
class QUrl;

class MdiArea;
class MdiDocument;
class MdiWindow;


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    bool openDocument(const QUrl &url);

signals:
    void actionsIsEnabled(const bool enabled);
    void actionsFileIsEnabled(const bool enabled);
    void actionCloseOtherIsEnabled(const bool enabled);

protected:
    void closeEvent(QCloseEvent *event) override;

private:
    void setupActions();
    void updateActionsToolButtonStyle(const Qt::ToolButtonStyle toolButtonStyle);
    void updateActionViewFullScreen();

    void loadSettings();
    void saveSettings();

    MdiDocument *createDocument();
    MdiDocument *extractDocument(const QMdiSubWindow *subWindow) const;
    MdiDocument *activeDocument() const;
    bool loadDocument(const QUrl &url);
    bool saveDocument(MdiDocument *document, const QUrl &url);

private slots:
    void enableActions(QMdiSubWindow *subWindow = nullptr);
    void enableActionsFile();
    void enableActionCloseOther();

    void activateDocument(QMdiSubWindow *subWindow);

    void updateWindowTitle();

    void onActionAboutTriggered();
    void onActionColophonTriggered();
    void onActionPreferencesTriggered();

    void onActionNewTriggered();
    void onActionOpenTriggered();
    void onActionSaveTriggered();
    void onActionSaveAsTriggered();
    void onActionSaveCopyAsTriggered();
    void onActionSaveAllTriggered();
    void onActionCopyPathTriggered();
    void onActionRenameTriggered();
    void onActionCloseOtherTriggered(QMdiSubWindow *subWindow = nullptr);
    void onActionCloseAllTriggered();

    void onActionsToolButtonStyleTriggered(const QAction *actionToolButtonStyle);
    void onActionViewFullScreenTriggered(const bool checked);

private:
    MdiArea *m_documentsArea;

    QAction *m_actionAbout;
    QAction *m_actionColophon;
    QAction *m_actionPreferences;
    QAction *m_actionQuit;
    QToolBar *m_toolbarApplication;

    QAction *m_actionNew;
    QAction *m_actionOpen;
    QAction *m_actionSave;
    QAction *m_actionSaveAs;
    QAction *m_actionSaveCopyAs;
    QAction *m_actionSaveAll;
    QAction *m_actionCopyPath;
    QAction *m_actionRename;
    QAction *m_actionClose;
    QAction *m_actionCloseOther;
    QAction *m_actionCloseAll;
    QToolBar *m_toolbarFile;

    QToolBar *m_toolbarEdit;

    QToolBar *m_toolbarView;

    QToolBar *m_toolbarFormat;

    QToolBar *m_toolbarTools;

    QAction *m_actionShowPath;
    QAction *m_actionShowMenubar;
    QAction *m_actionShowToolbarApplication;
    QAction *m_actionShowToolbarFile;
    QAction *m_actionShowToolbarEdit;
    QAction *m_actionShowToolbarView;
    QAction *m_actionShowToolbarFormat;
    QAction *m_actionShowToolbarTools;
    QAction *m_actionShowToolbarAppearance;
    QAction *m_actionShowToolbarHelp;
    QActionGroup *m_actionsToolButtonStyle;
    QAction *m_actionShowStatusbar;
    QAction *m_actionViewFullScreen;
    QToolBar *m_toolbarAppearance;

    QToolBar *m_toolbarHelp;
};

#endif // MAIN_WINDOW_H
