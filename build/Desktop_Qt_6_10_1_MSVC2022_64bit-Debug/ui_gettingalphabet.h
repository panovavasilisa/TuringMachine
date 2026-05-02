/********************************************************************************
** Form generated from reading UI file 'gettingalphabet.ui'
**
** Created by: Qt User Interface Compiler version 6.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GETTINGALPHABET_H
#define UI_GETTINGALPHABET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_GettingAlphabet
{
public:
    QPushButton *btnSetAlphabet;
    QLineEdit *lineEditAlphabet;
    QLineEdit *lineEditExtra;
    QLabel *label;
    QLabel *label_2;

    void setupUi(QDialog *GettingAlphabet)
    {
        if (GettingAlphabet->objectName().isEmpty())
            GettingAlphabet->setObjectName("GettingAlphabet");
        GettingAlphabet->resize(400, 300);
        btnSetAlphabet = new QPushButton(GettingAlphabet);
        btnSetAlphabet->setObjectName("btnSetAlphabet");
        btnSetAlphabet->setGeometry(QRect(130, 170, 121, 41));
        lineEditAlphabet = new QLineEdit(GettingAlphabet);
        lineEditAlphabet->setObjectName("lineEditAlphabet");
        lineEditAlphabet->setGeometry(QRect(40, 120, 113, 20));
        lineEditExtra = new QLineEdit(GettingAlphabet);
        lineEditExtra->setObjectName("lineEditExtra");
        lineEditExtra->setGeometry(QRect(220, 120, 113, 20));
        label = new QLabel(GettingAlphabet);
        label->setObjectName("label");
        label->setGeometry(QRect(40, 70, 121, 31));
        label_2 = new QLabel(GettingAlphabet);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(220, 70, 131, 31));

        retranslateUi(GettingAlphabet);

        QMetaObject::connectSlotsByName(GettingAlphabet);
    } // setupUi

    void retranslateUi(QDialog *GettingAlphabet)
    {
        GettingAlphabet->setWindowTitle(QCoreApplication::translate("GettingAlphabet", "Dialog", nullptr));
        btnSetAlphabet->setText(QCoreApplication::translate("GettingAlphabet", "\320\227\320\260\320\264\320\260\321\202\321\214 \320\260\320\273\321\204\320\260\320\262\320\270\321\202\321\213 :", nullptr));
        label->setText(QCoreApplication::translate("GettingAlphabet", "\320\220\320\273\321\204\320\260\320\262\320\270\321\202 \321\201\321\202\321\200\320\276\320\272\320\270 :", nullptr));
        label_2->setText(QCoreApplication::translate("GettingAlphabet", "\320\220\320\273\321\204\320\260\320\262\320\270\321\202 \320\264\320\276\320\277. \321\201\320\270\320\274\320\262\320\276\320\273\320\276\320\262 :", nullptr));
    } // retranslateUi

};

namespace Ui {
    class GettingAlphabet: public Ui_GettingAlphabet {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GETTINGALPHABET_H
