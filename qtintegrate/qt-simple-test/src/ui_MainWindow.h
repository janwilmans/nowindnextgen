/********************************************************************************
** Form generated from reading ui file 'MainWindow.ui'
**
** Created: Thu 8. Oct 22:47:59 2009
**      by: Qt User Interface Compiler version 4.4.3
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHBoxLayout>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QStatusBar>
#include <QtGui/QWidget>
#include "./GLWidget.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *action_Quit;
    QAction *action_foo1;
    QAction *action_foo2;
    QAction *action_foo3;
    QAction *action_foo4;
    QAction *action_foo5;
    QAction *action_foo6;
    QAction *action_foo7;
    QAction *action_foo8;
    QAction *action_foo9;
    QWidget *centralwidget;
    QHBoxLayout *hboxLayout;
    GLWidget *globject;
    QMenuBar *menubar;
    QMenu *menu_File;
    QMenu *menu_Edit;
    QMenu *menu_Advanced;
    QMenu *menu_ViewSettings;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
    if (MainWindow->objectName().isEmpty())
        MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
    MainWindow->resize(478, 470);
    action_Quit = new QAction(MainWindow);
    action_Quit->setObjectName(QString::fromUtf8("action_Quit"));
    action_foo1 = new QAction(MainWindow);
    action_foo1->setObjectName(QString::fromUtf8("action_foo1"));
    action_foo2 = new QAction(MainWindow);
    action_foo2->setObjectName(QString::fromUtf8("action_foo2"));
    action_foo3 = new QAction(MainWindow);
    action_foo3->setObjectName(QString::fromUtf8("action_foo3"));
    action_foo4 = new QAction(MainWindow);
    action_foo4->setObjectName(QString::fromUtf8("action_foo4"));
    action_foo5 = new QAction(MainWindow);
    action_foo5->setObjectName(QString::fromUtf8("action_foo5"));
    action_foo6 = new QAction(MainWindow);
    action_foo6->setObjectName(QString::fromUtf8("action_foo6"));
    action_foo7 = new QAction(MainWindow);
    action_foo7->setObjectName(QString::fromUtf8("action_foo7"));
    action_foo8 = new QAction(MainWindow);
    action_foo8->setObjectName(QString::fromUtf8("action_foo8"));
    action_foo9 = new QAction(MainWindow);
    action_foo9->setObjectName(QString::fromUtf8("action_foo9"));
    centralwidget = new QWidget(MainWindow);
    centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
    hboxLayout = new QHBoxLayout(centralwidget);
    hboxLayout->setSpacing(6);
    hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
    hboxLayout->setContentsMargins(9, 9, 9, 9);
    globject = new GLWidget(centralwidget);
    globject->setObjectName(QString::fromUtf8("globject"));

    hboxLayout->addWidget(globject);

    MainWindow->setCentralWidget(centralwidget);
    menubar = new QMenuBar(MainWindow);
    menubar->setObjectName(QString::fromUtf8("menubar"));
    menubar->setGeometry(QRect(0, 0, 78, 19));
    menu_File = new QMenu(menubar);
    menu_File->setObjectName(QString::fromUtf8("menu_File"));
    menu_Edit = new QMenu(menubar);
    menu_Edit->setObjectName(QString::fromUtf8("menu_Edit"));
    menu_Advanced = new QMenu(menubar);
    menu_Advanced->setObjectName(QString::fromUtf8("menu_Advanced"));
    menu_ViewSettings = new QMenu(menubar);
    menu_ViewSettings->setObjectName(QString::fromUtf8("menu_ViewSettings"));
    MainWindow->setMenuBar(menubar);
    statusbar = new QStatusBar(MainWindow);
    statusbar->setObjectName(QString::fromUtf8("statusbar"));
    MainWindow->setStatusBar(statusbar);

    menubar->addAction(menu_File->menuAction());
    menubar->addAction(menu_Edit->menuAction());
    menubar->addAction(menu_Advanced->menuAction());
    menubar->addAction(menu_ViewSettings->menuAction());
    menu_File->addAction(action_Quit);
    menu_Advanced->addAction(action_foo1);
    menu_Advanced->addAction(action_foo2);
    menu_Advanced->addAction(action_foo3);
    menu_Advanced->addAction(action_foo4);
    menu_Advanced->addAction(action_foo5);
    menu_Advanced->addAction(action_foo6);
    menu_Advanced->addAction(action_foo7);
    menu_Advanced->addAction(action_foo8);
    menu_Advanced->addAction(action_foo9);
    menu_ViewSettings->addAction(action_foo1);
    menu_ViewSettings->addAction(action_foo2);
    menu_ViewSettings->addAction(action_foo3);
    menu_ViewSettings->addAction(action_foo4);
    menu_ViewSettings->addAction(action_foo5);
    menu_ViewSettings->addAction(action_foo6);
    menu_ViewSettings->addAction(action_foo7);
    menu_ViewSettings->addAction(action_foo8);
    menu_ViewSettings->addAction(action_foo9);

    retranslateUi(MainWindow);

    QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
    MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0, QApplication::UnicodeUTF8));
    action_Quit->setText(QApplication::translate("MainWindow", "&Quit", 0, QApplication::UnicodeUTF8));
    action_foo1->setText(QApplication::translate("MainWindow", "foofoofoofoo&1", 0, QApplication::UnicodeUTF8));
    action_foo2->setText(QApplication::translate("MainWindow", "foofoofoofoo&2", 0, QApplication::UnicodeUTF8));
    action_foo3->setText(QApplication::translate("MainWindow", "foofoofoofoo&3", 0, QApplication::UnicodeUTF8));
    action_foo4->setText(QApplication::translate("MainWindow", "foofoofoofoo&4", 0, QApplication::UnicodeUTF8));
    action_foo5->setText(QApplication::translate("MainWindow", "foofoofoofoo&5", 0, QApplication::UnicodeUTF8));
    action_foo6->setText(QApplication::translate("MainWindow", "foofoofoofoo&6", 0, QApplication::UnicodeUTF8));
    action_foo7->setText(QApplication::translate("MainWindow", "foofoofoofoo&7", 0, QApplication::UnicodeUTF8));
    action_foo8->setText(QApplication::translate("MainWindow", "foofoofoofoo&8", 0, QApplication::UnicodeUTF8));
    action_foo9->setText(QApplication::translate("MainWindow", "foofoofoofoo&9", 0, QApplication::UnicodeUTF8));
    menu_File->setTitle(QApplication::translate("MainWindow", "&File", 0, QApplication::UnicodeUTF8));
    menu_Edit->setTitle(QApplication::translate("MainWindow", "&Edit", 0, QApplication::UnicodeUTF8));
    menu_Advanced->setTitle(QApplication::translate("MainWindow", "&Advanced Options", 0, QApplication::UnicodeUTF8));
    menu_ViewSettings->setTitle(QApplication::translate("MainWindow", "&View Settings", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
