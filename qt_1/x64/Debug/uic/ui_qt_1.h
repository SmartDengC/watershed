/********************************************************************************
** Form generated from reading UI file 'qt_1.ui'
**
** Created by: Qt User Interface Compiler version 5.12.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QT_1_H
#define UI_QT_1_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Qt_1Class
{
public:

    void setupUi(QWidget *Qt_1Class)
    {
        if (Qt_1Class->objectName().isEmpty())
            Qt_1Class->setObjectName(QString::fromUtf8("Qt_1Class"));
        Qt_1Class->resize(600, 400);

        retranslateUi(Qt_1Class);

        QMetaObject::connectSlotsByName(Qt_1Class);
    } // setupUi

    void retranslateUi(QWidget *Qt_1Class)
    {
        Qt_1Class->setWindowTitle(QApplication::translate("Qt_1Class", "Qt_1", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Qt_1Class: public Ui_Qt_1Class {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QT_1_H
