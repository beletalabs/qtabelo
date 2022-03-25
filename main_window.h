/**
 * Copyright 2022 naracanto <https://naracanto.github.io>.
 *
 * This file is part of Tabelo-Qt <https://github.com/cutelabs/tabelo-qt>.
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


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    bool openDocument(const QUrl &url);

protected:
    void closeEvent(QCloseEvent *event) override;

private:
    void setupActions();

    void updateActionsToolButtonStyle(const Qt::ToolButtonStyle toolButtonStyle);
    void updateActionFullScreen();

    void loadSettings();
    void saveSettings();

    MdiDocument *createDocument();
    MdiDocument *extractDocument(const QMdiSubWindow *subWindow) const;
    MdiDocument *activeDocument() const;
    bool loadDocument(const QUrl &url);
    bool saveDocument(const MdiDocument *document, const QUrl &url);

private slots:
    void updateWindowCaption(const QMdiSubWindow *subWindow);

    void onActionAboutTriggered();
    void onActionColophonTriggered();
    void onActionPreferencesTriggered();

    void onActionNewTriggered();
    void onActionOpenTriggered();
    void onActionSaveTriggered();
    void onActionSaveAsTriggered();
    void onActionSaveCopyAsTriggered();
    void onActionSaveAllTriggered();
    void onActionCloseOtherTriggered();
    void onActionCloseAllTriggered();

    void onActionsToolButtonStyleTriggered(const QAction *actionToolButtonStyle);
    void onActionFullScreenTriggered();

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
    QAction *m_actionClose;
    QAction *m_actionCloseOther;
    QAction *m_actionCloseAll;
    QToolBar *m_toolbarFile;

    QToolBar *m_toolbarEdit;

    QToolBar *m_toolbarView;

    QToolBar *m_toolbarFormat;

    QToolBar *m_toolbarTools;

    QAction *m_actionFullPath;
    QAction *m_actionMenubar;
    QAction *m_actionToolbarApplication;
    QAction *m_actionToolbarFile;
    QAction *m_actionToolbarEdit;
    QAction *m_actionToolbarView;
    QAction *m_actionToolbarFormat;
    QAction *m_actionToolbarTools;
    QAction *m_actionToolbarAppearance;
    QAction *m_actionToolbarHelp;
    QActionGroup *m_actionsToolButtonStyle;
    QAction *m_actionStatusbar;
    QAction *m_actionFullScreen;
    QToolBar *m_toolbarAppearance;

    QToolBar *m_toolbarHelp;

    QStatusBar *m_statusbar;
};

#endif // MAIN_WINDOW_H
