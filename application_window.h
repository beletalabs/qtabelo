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

#ifndef APPLICATION_WINDOW_H
#define APPLICATION_WINDOW_H

#include <QMainWindow>

class QAction;
class QActionGroup;
class QCloseEvent;
class QMdiSubWindow;
class QStatusBar;
class QToolBar;
class QUrl;

class DocumentManager;
class DocumentWidget;


class ApplicationWindow : public QMainWindow
{
    Q_OBJECT

public:
    ApplicationWindow(QWidget *parent = nullptr);
    ~ApplicationWindow();

    bool openDocument(const QUrl &url);

signals:
    void documentCountChanged(const int count);

protected:
    void closeEvent(QCloseEvent *event) override;

private:
    void setupActions();
    void updateActionsToolButtonStyle(const Qt::ToolButtonStyle style);
    void updateActionFullScreen();

    void enableActions(const bool enabled);
    void enableActionCloseOther(const bool enabled);
    void enableUrlActions(const bool enabled);
    void enableFileActions(const bool enabled);

    void loadSettings();
    void saveSettings();

    void updateWindowModified();
    void updateWindowTitle();

    DocumentWidget *createDocument();
    bool loadDocument(const QUrl &url);
    bool saveDocument(DocumentWidget *document, const QUrl &altUrl);

private slots:
    void documentCreated();
    void documentActivated(QMdiSubWindow *subWindow);
    void documentModifiedChanged(const bool modified);
    void documentUrlChanged(const QUrl &url);
    void documentClosed();

private:
    DocumentWidget *extractDocument(const QMdiSubWindow *subWindow) const;
    DocumentWidget *activeDocument() const;
    bool hasActiveDocument() const;
    bool hasActiveDocumentUrl() const;
    bool hasActiveDocumentUrlFile() const;

private slots:
    void slotAbout();
    void slotColophon();
    void slotPreferences();

    void slotNew();
    void slotOpen();
    void slotSave();
    void slotSaveAs();
    void slotSaveCopyAs();
    void slotSaveAll();
    void slotCopyPath();
    void slotRenameFilename();
    void slotCloseOther();
    void slotCloseAll();

    void slotToolButtonStyle(const QAction *action);
    void slotFullScreen(const bool checked);

private:
    DocumentManager *m_documentManager;

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
    QAction *m_actionRenameFilename;
    QAction *m_actionClose;
    QAction *m_actionCloseOther;
    QAction *m_actionCloseAll;
    QToolBar *m_toolbarDocument;

    QToolBar *m_toolbarEdit;

    QToolBar *m_toolbarView;

    QToolBar *m_toolbarFormat;

    QToolBar *m_toolbarTools;

    QAction *m_actionShowPath;
    QAction *m_actionShowMenubar;
    QAction *m_actionShowToolbarApplication;
    QAction *m_actionShowToolbarDocument;
    QAction *m_actionShowToolbarEdit;
    QAction *m_actionShowToolbarView;
    QAction *m_actionShowToolbarFormat;
    QAction *m_actionShowToolbarTools;
    QAction *m_actionShowToolbarSettings;
    QAction *m_actionShowToolbarHelp;
    QActionGroup *m_actionsToolButtonStyle;
    QAction *m_actionShowStatusbar;
    QAction *m_actionFullScreen;
    QToolBar *m_toolbarSettings;

    QToolBar *m_toolbarHelp;
};

#endif // APPLICATION_WINDOW_H
