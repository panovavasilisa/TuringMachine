/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QWidget>
#include "tapewidget.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QTableWidget *tableProgram;
    QPushButton *btnSetString;
    QLineEdit *lineEditString;
    QLabel *label;
    QPushButton *btnChangeAlphabet;
    QLabel *label_2;
    QWidget *horizontalLayoutWidget;
    QHBoxLayout *horizontalLayout;
    QPushButton *btnStep;
    QPushButton *btnRun;
    QPushButton *btnStop;
    QPushButton *btnReset;
    QSlider *Speed;
    TapeWidget *tapeWidget;
    QPushButton *btnAddState;
    QPushButton *btnRemoveState;
    QPushButton *btnInsertEmpty;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(800, 600);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        tableProgram = new QTableWidget(centralwidget);
        tableProgram->setObjectName("tableProgram");
        tableProgram->setGeometry(QRect(20, 140, 761, 192));
        btnSetString = new QPushButton(centralwidget);
        btnSetString->setObjectName("btnSetString");
        btnSetString->setGeometry(QRect(260, 30, 121, 31));
        lineEditString = new QLineEdit(centralwidget);
        lineEditString->setObjectName("lineEditString");
        lineEditString->setGeometry(QRect(20, 40, 201, 20));
        label = new QLabel(centralwidget);
        label->setObjectName("label");
        label->setGeometry(QRect(30, 10, 111, 31));
        btnChangeAlphabet = new QPushButton(centralwidget);
        btnChangeAlphabet->setObjectName("btnChangeAlphabet");
        btnChangeAlphabet->setGeometry(QRect(20, 380, 161, 21));
        label_2 = new QLabel(centralwidget);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(220, 380, 61, 21));
        horizontalLayoutWidget = new QWidget(centralwidget);
        horizontalLayoutWidget->setObjectName("horizontalLayoutWidget");
        horizontalLayoutWidget->setGeometry(QRect(20, 410, 411, 31));
        horizontalLayout = new QHBoxLayout(horizontalLayoutWidget);
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        btnStep = new QPushButton(horizontalLayoutWidget);
        btnStep->setObjectName("btnStep");

        horizontalLayout->addWidget(btnStep);

        btnRun = new QPushButton(horizontalLayoutWidget);
        btnRun->setObjectName("btnRun");

        horizontalLayout->addWidget(btnRun);

        btnStop = new QPushButton(horizontalLayoutWidget);
        btnStop->setObjectName("btnStop");

        horizontalLayout->addWidget(btnStop);

        btnReset = new QPushButton(horizontalLayoutWidget);
        btnReset->setObjectName("btnReset");

        horizontalLayout->addWidget(btnReset);

        Speed = new QSlider(centralwidget);
        Speed->setObjectName("Speed");
        Speed->setGeometry(QRect(280, 380, 151, 20));
        Speed->setSingleStep(1);
        Speed->setSliderPosition(50);
        Speed->setOrientation(Qt::Orientation::Horizontal);
        tapeWidget = new TapeWidget(centralwidget);
        tapeWidget->setObjectName("tapeWidget");
        tapeWidget->setGeometry(QRect(19, 80, 761, 21));
        btnAddState = new QPushButton(centralwidget);
        btnAddState->setObjectName("btnAddState");
        btnAddState->setGeometry(QRect(20, 340, 21, 18));
        btnRemoveState = new QPushButton(centralwidget);
        btnRemoveState->setObjectName("btnRemoveState");
        btnRemoveState->setGeometry(QRect(50, 340, 21, 18));
        btnInsertEmpty = new QPushButton(centralwidget);
        btnInsertEmpty->setObjectName("btnInsertEmpty");
        btnInsertEmpty->setGeometry(QRect(110, 340, 71, 18));
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 800, 18));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        btnSetString->setText(QCoreApplication::translate("MainWindow", "\320\227\320\260\320\264\320\260\321\202\321\214 \321\201\321\202\321\200\320\276\320\272\321\203 :", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "\320\241\321\202\321\200\320\276\320\272\320\260 \320\275\320\260 \320\273\320\265\320\275\321\202\320\265 :", nullptr));
        btnChangeAlphabet->setText(QCoreApplication::translate("MainWindow", "\320\230\320\267\320\274\320\265\320\275\320\270\321\202\321\214 \320\260\320\273\321\204\320\260\320\262\320\270\321\202\321\213 :", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "\320\241\320\272\320\276\321\200\320\276\321\201\321\202\321\214", nullptr));
        btnStep->setText(QString());
        btnRun->setText(QString());
        btnStop->setText(QString());
        btnReset->setText(QString());
        btnAddState->setText(QCoreApplication::translate("MainWindow", "+", nullptr));
        btnRemoveState->setText(QCoreApplication::translate("MainWindow", "-", nullptr));
        btnInsertEmpty->setText(QCoreApplication::translate("MainWindow", "\320\222\321\201\321\202\320\260\320\262\320\270\321\202\321\214 \316\233", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
