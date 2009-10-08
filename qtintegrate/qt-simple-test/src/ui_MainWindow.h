/********************************************************************************
** Form generated from reading ui file 'MainWindow.ui'
**
** Created: Thu 8. Oct 21:28:43 2009
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
#include <glwidget.h>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *action_Quit;
    QWidget *centralwidget;
    QHBoxLayout *hboxLayout;
    GLWidget *globject;
    QMenuBar *menubar;
    QMenu *menu_File;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
    if (MainWindow->objectName().isEmpty())
        MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
    MainWindow->resize(478, 470);
    action_Quit = new QAction(MainWindow);
    action_Quit->setObjectName(QString::fromUtf8("action_Quit"));
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
    MainWindow->setMenuBar(menubar);
    statusbar = new QStatusBar(MainWindow);
    statusbar->setObjectName(QString::fromUtf8("statusbar"));
    MainWindow->setStatusBar(statusbar);

    menubar->addAction(menu_File->menuAction());

    retranslateUi(MainWindow);

    QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
    MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0, QApplication::UnicodeUTF8));
    action_Quit->setText(QApplication::translate("MainWindow", "&Quit", 0, QApplication::UnicodeUTF8));
    menu_File->setTitle(QApplication::translate("MainWindow", "&File", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
