/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.2.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionExit;
    QAction *actionExit_2;
    QWidget *centralWidget;
    QPushButton *pushButton;
    QLabel *label_name;
    QLabel *label_age;
    QLabel *label_Gender;
    QLineEdit *lineEdit_name;
    QSpinBox *age;
    QGroupBox *groupBox;
    QRadioButton *radioButton_male;
    QRadioButton *radioButton_female;
    QLabel *label;
    QLabel *label_3;
    QLabel *label_2;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;
    QMenuBar *menuBar;
    QMenu *menuFile;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(304, 347);
        actionExit = new QAction(MainWindow);
        actionExit->setObjectName(QStringLiteral("actionExit"));
        actionExit_2 = new QAction(MainWindow);
        actionExit_2->setObjectName(QStringLiteral("actionExit_2"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        pushButton = new QPushButton(centralWidget);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(110, 250, 85, 27));
        label_name = new QLabel(centralWidget);
        label_name->setObjectName(QStringLiteral("label_name"));
        label_name->setGeometry(QRect(30, 90, 67, 17));
        label_name->setMinimumSize(QSize(67, 17));
        label_age = new QLabel(centralWidget);
        label_age->setObjectName(QStringLiteral("label_age"));
        label_age->setGeometry(QRect(30, 123, 71, 17));
        label_Gender = new QLabel(centralWidget);
        label_Gender->setObjectName(QStringLiteral("label_Gender"));
        label_Gender->setGeometry(QRect(30, 156, 71, 17));
        lineEdit_name = new QLineEdit(centralWidget);
        lineEdit_name->setObjectName(QStringLiteral("lineEdit_name"));
        lineEdit_name->setGeometry(QRect(120, 90, 146, 27));
        age = new QSpinBox(centralWidget);
        age->setObjectName(QStringLiteral("age"));
        age->setGeometry(QRect(120, 120, 48, 27));
        groupBox = new QGroupBox(centralWidget);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(120, 150, 276, 28));
        radioButton_male = new QRadioButton(groupBox);
        radioButton_male->setObjectName(QStringLiteral("radioButton_male"));
        radioButton_male->setGeometry(QRect(0, 0, 61, 21));
        radioButton_female = new QRadioButton(groupBox);
        radioButton_female->setObjectName(QStringLiteral("radioButton_female"));
        radioButton_female->setGeometry(QRect(70, 0, 81, 22));
        label = new QLabel(centralWidget);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(30, 20, 161, 51));
        label->setMaximumSize(QSize(16777215, 16777215));
        label->setSizeIncrement(QSize(500, 500));
        label->setBaseSize(QSize(500, 500));
        QFont font;
        font.setFamily(QStringLiteral("Latin Modern Roman Unslanted"));
        font.setPointSize(18);
        label->setFont(font);
        label_3 = new QLabel(centralWidget);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(180, 10, 111, 71));
        label_2 = new QLabel(centralWidget);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(-10, 0, 961, 651));
        MainWindow->setCentralWidget(centralWidget);
        label_2->raise();
        pushButton->raise();
        label_name->raise();
        label_age->raise();
        label_Gender->raise();
        lineEdit_name->raise();
        age->raise();
        groupBox->raise();
        label->raise();
        label_3->raise();
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::BottomToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 304, 25));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        MainWindow->setMenuBar(menuBar);

        menuBar->addAction(menuFile->menuAction());
        menuFile->addAction(actionExit_2);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0));
        actionExit->setText(QApplication::translate("MainWindow", "Exit", 0));
        actionExit_2->setText(QApplication::translate("MainWindow", "Exit", 0));
        pushButton->setText(QApplication::translate("MainWindow", "Start", 0));
        label_name->setText(QApplication::translate("MainWindow", "Name      :", 0));
        label_age->setText(QApplication::translate("MainWindow", "Age          :", 0));
        label_Gender->setText(QApplication::translate("MainWindow", "Gender   :", 0));
        groupBox->setTitle(QString());
        radioButton_male->setText(QApplication::translate("MainWindow", "Male", 0));
        radioButton_female->setText(QApplication::translate("MainWindow", "Female", 0));
        label->setText(QApplication::translate("MainWindow", "Pupil Hacker", 0));
        label_3->setText(QString());
        label_2->setText(QString());
        menuFile->setTitle(QApplication::translate("MainWindow", "File", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
