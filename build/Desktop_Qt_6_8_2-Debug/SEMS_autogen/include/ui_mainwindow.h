/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTimeEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QWidget *gridLayoutWidget;
    QGridLayout *gridLayout_2;
    QGroupBox *groupBox;
    QWidget *Solar;
    QLabel *label;
    QWidget *gridLayoutWidget_2;
    QGridLayout *gridLayout_3;
    QGroupBox *groupBox_2;
    QWidget *gridLayoutWidget_3;
    QGridLayout *gridLayout_4;
    QGroupBox *groupBox_3;
    QWidget *gridLayoutWidget_4;
    QGridLayout *gridLayout_5;
    QGroupBox *groupBox_4;
    QWidget *formLayoutWidget;
    QFormLayout *formLayout;
    QLabel *navLabel;
    QLineEdit *navLineEdit;
    QWidget *gridLayoutWidget_5;
    QGridLayout *gridLayout_6;
    QTimeEdit *timeEdit;
    QWidget *gridLayoutWidget_6;
    QGridLayout *gridLayout_7;
    QGroupBox *groupBox_5;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1169, 708);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        gridLayoutWidget = new QWidget(centralwidget);
        gridLayoutWidget->setObjectName("gridLayoutWidget");
        gridLayoutWidget->setGeometry(QRect(0, 0, 321, 181));
        gridLayout_2 = new QGridLayout(gridLayoutWidget);
        gridLayout_2->setObjectName("gridLayout_2");
        gridLayout_2->setContentsMargins(0, 0, 0, 0);
        groupBox = new QGroupBox(gridLayoutWidget);
        groupBox->setObjectName("groupBox");
        Solar = new QWidget(groupBox);
        Solar->setObjectName("Solar");
        Solar->setGeometry(QRect(0, 30, 311, 101));
        label = new QLabel(groupBox);
        label->setObjectName("label");
        label->setGeometry(QRect(1, 140, 311, 31));

        gridLayout_2->addWidget(groupBox, 0, 0, 1, 1);

        gridLayoutWidget_2 = new QWidget(centralwidget);
        gridLayoutWidget_2->setObjectName("gridLayoutWidget_2");
        gridLayoutWidget_2->setGeometry(QRect(320, 0, 261, 181));
        gridLayout_3 = new QGridLayout(gridLayoutWidget_2);
        gridLayout_3->setObjectName("gridLayout_3");
        gridLayout_3->setContentsMargins(0, 0, 0, 0);
        groupBox_2 = new QGroupBox(gridLayoutWidget_2);
        groupBox_2->setObjectName("groupBox_2");

        gridLayout_3->addWidget(groupBox_2, 0, 0, 1, 1);

        gridLayoutWidget_3 = new QWidget(centralwidget);
        gridLayoutWidget_3->setObjectName("gridLayoutWidget_3");
        gridLayoutWidget_3->setGeometry(QRect(580, 0, 251, 181));
        gridLayout_4 = new QGridLayout(gridLayoutWidget_3);
        gridLayout_4->setObjectName("gridLayout_4");
        gridLayout_4->setContentsMargins(0, 0, 0, 0);
        groupBox_3 = new QGroupBox(gridLayoutWidget_3);
        groupBox_3->setObjectName("groupBox_3");

        gridLayout_4->addWidget(groupBox_3, 0, 0, 1, 1);

        gridLayoutWidget_4 = new QWidget(centralwidget);
        gridLayoutWidget_4->setObjectName("gridLayoutWidget_4");
        gridLayoutWidget_4->setGeometry(QRect(830, 0, 241, 181));
        gridLayout_5 = new QGridLayout(gridLayoutWidget_4);
        gridLayout_5->setObjectName("gridLayout_5");
        gridLayout_5->setContentsMargins(0, 0, 0, 0);
        groupBox_4 = new QGroupBox(gridLayoutWidget_4);
        groupBox_4->setObjectName("groupBox_4");

        gridLayout_5->addWidget(groupBox_4, 0, 0, 1, 1);

        formLayoutWidget = new QWidget(centralwidget);
        formLayoutWidget->setObjectName("formLayoutWidget");
        formLayoutWidget->setGeometry(QRect(1070, 0, 81, 601));
        formLayout = new QFormLayout(formLayoutWidget);
        formLayout->setObjectName("formLayout");
        formLayout->setContentsMargins(0, 0, 0, 0);
        navLabel = new QLabel(formLayoutWidget);
        navLabel->setObjectName("navLabel");

        formLayout->setWidget(0, QFormLayout::LabelRole, navLabel);

        navLineEdit = new QLineEdit(formLayoutWidget);
        navLineEdit->setObjectName("navLineEdit");

        formLayout->setWidget(0, QFormLayout::FieldRole, navLineEdit);

        gridLayoutWidget_5 = new QWidget(centralwidget);
        gridLayoutWidget_5->setObjectName("gridLayoutWidget_5");
        gridLayoutWidget_5->setGeometry(QRect(0, 570, 1071, 32));
        gridLayout_6 = new QGridLayout(gridLayoutWidget_5);
        gridLayout_6->setObjectName("gridLayout_6");
        gridLayout_6->setContentsMargins(0, 0, 0, 0);
        timeEdit = new QTimeEdit(gridLayoutWidget_5);
        timeEdit->setObjectName("timeEdit");

        gridLayout_6->addWidget(timeEdit, 0, 0, 1, 1);

        gridLayoutWidget_6 = new QWidget(centralwidget);
        gridLayoutWidget_6->setObjectName("gridLayoutWidget_6");
        gridLayoutWidget_6->setGeometry(QRect(-1, 179, 1071, 391));
        gridLayout_7 = new QGridLayout(gridLayoutWidget_6);
        gridLayout_7->setObjectName("gridLayout_7");
        gridLayout_7->setContentsMargins(0, 0, 0, 0);
        groupBox_5 = new QGroupBox(gridLayoutWidget_6);
        groupBox_5->setObjectName("groupBox_5");

        gridLayout_7->addWidget(groupBox_5, 0, 0, 1, 1);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1169, 26));
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
        groupBox->setTitle(QCoreApplication::translate("MainWindow", " Solar Panel (W)", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "Solar (W)", nullptr));
        groupBox_2->setTitle(QCoreApplication::translate("MainWindow", "Energy Generated", nullptr));
        groupBox_3->setTitle(QCoreApplication::translate("MainWindow", "House Load", nullptr));
        groupBox_4->setTitle(QCoreApplication::translate("MainWindow", "Battery Power", nullptr));
        navLabel->setText(QCoreApplication::translate("MainWindow", "Nav", nullptr));
        groupBox_5->setTitle(QCoreApplication::translate("MainWindow", "PowerChart", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
