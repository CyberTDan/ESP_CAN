/********************************************************************************
** Form generated from reading UI file 'canoutputwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.12.8
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CANOUTPUTWINDOW_H
#define UI_CANOUTPUTWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CanOutputWindow
{
public:
    QWidget *centralwidget;
    QPushButton *pushButton;
    QTableWidget *tableWidget;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *CanOutputWindow)
    {
        if (CanOutputWindow->objectName().isEmpty())
            CanOutputWindow->setObjectName(QString::fromUtf8("CanOutputWindow"));
        CanOutputWindow->resize(810, 570);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(CanOutputWindow->sizePolicy().hasHeightForWidth());
        CanOutputWindow->setSizePolicy(sizePolicy);
        CanOutputWindow->setMinimumSize(QSize(810, 570));
        CanOutputWindow->setMaximumSize(QSize(810, 570));
        centralwidget = new QWidget(CanOutputWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        pushButton = new QPushButton(centralwidget);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(700, 510, 101, 21));
        tableWidget = new QTableWidget(centralwidget);
        tableWidget->setObjectName(QString::fromUtf8("tableWidget"));
        tableWidget->setGeometry(QRect(5, 1, 800, 500));
        sizePolicy.setHeightForWidth(tableWidget->sizePolicy().hasHeightForWidth());
        tableWidget->setSizePolicy(sizePolicy);
        tableWidget->setMinimumSize(QSize(800, 500));
        tableWidget->setMaximumSize(QSize(800, 500));
        CanOutputWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(CanOutputWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 810, 22));
        CanOutputWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(CanOutputWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        CanOutputWindow->setStatusBar(statusbar);

        retranslateUi(CanOutputWindow);

        QMetaObject::connectSlotsByName(CanOutputWindow);
    } // setupUi

    void retranslateUi(QMainWindow *CanOutputWindow)
    {
        CanOutputWindow->setWindowTitle(QApplication::translate("CanOutputWindow", "CAN Receiver", nullptr));
        pushButton->setText(QApplication::translate("CanOutputWindow", "Clear", nullptr));
    } // retranslateUi

};

namespace Ui {
    class CanOutputWindow: public Ui_CanOutputWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CANOUTPUTWINDOW_H
