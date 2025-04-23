/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QWidget *container3DWidget;
    QStackedWidget *stackedWidget;
    QWidget *programsPage;
    QPushButton *cottonsButton;
    QPushButton *cottonsEcoButton;
    QPushButton *syntheticsButton;
    QPushButton *woolSilkButton;
    QPushButton *antiAllergyButton;
    QPushButton *nonStopButton;
    QPushButton *antiCreaseButton;
    QPushButton *refreshButton;
    QWidget *insertPowderPage;
    QPushButton *pushButton;
    QWidget *cottonsDescPage;
    QLabel *title;
    QTextEdit *textEdit;
    QComboBox *temperatureBox;
    QLabel *tempLabel;
    QComboBox *spinBox;
    QLabel *spinLabel;
    QComboBox *rinseBox;
    QLabel *rinseLabel;
    QLabel *ecomodeLabel;
    QComboBox *ecomodeBox;
    QPushButton *startButton;
    QPushButton *goBackButton;
    QLabel *hourLabel;
    QLabel *label_2;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1198, 615);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        container3DWidget = new QWidget(centralwidget);
        container3DWidget->setObjectName("container3DWidget");
        container3DWidget->setGeometry(QRect(10, 10, 931, 581));
        stackedWidget = new QStackedWidget(centralwidget);
        stackedWidget->setObjectName("stackedWidget");
        stackedWidget->setGeometry(QRect(950, 10, 231, 581));
        programsPage = new QWidget();
        programsPage->setObjectName("programsPage");
        cottonsButton = new QPushButton(programsPage);
        cottonsButton->setObjectName("cottonsButton");
        cottonsButton->setGeometry(QRect(0, 0, 231, 71));
        cottonsEcoButton = new QPushButton(programsPage);
        cottonsEcoButton->setObjectName("cottonsEcoButton");
        cottonsEcoButton->setGeometry(QRect(0, 70, 231, 71));
        syntheticsButton = new QPushButton(programsPage);
        syntheticsButton->setObjectName("syntheticsButton");
        syntheticsButton->setGeometry(QRect(0, 140, 231, 71));
        woolSilkButton = new QPushButton(programsPage);
        woolSilkButton->setObjectName("woolSilkButton");
        woolSilkButton->setGeometry(QRect(0, 210, 231, 71));
        antiAllergyButton = new QPushButton(programsPage);
        antiAllergyButton->setObjectName("antiAllergyButton");
        antiAllergyButton->setGeometry(QRect(0, 280, 231, 71));
        nonStopButton = new QPushButton(programsPage);
        nonStopButton->setObjectName("nonStopButton");
        nonStopButton->setGeometry(QRect(0, 350, 231, 71));
        antiCreaseButton = new QPushButton(programsPage);
        antiCreaseButton->setObjectName("antiCreaseButton");
        antiCreaseButton->setGeometry(QRect(0, 420, 231, 71));
        refreshButton = new QPushButton(programsPage);
        refreshButton->setObjectName("refreshButton");
        refreshButton->setGeometry(QRect(0, 490, 231, 71));
        stackedWidget->addWidget(programsPage);
        insertPowderPage = new QWidget();
        insertPowderPage->setObjectName("insertPowderPage");
        pushButton = new QPushButton(insertPowderPage);
        pushButton->setObjectName("pushButton");
        pushButton->setGeometry(QRect(0, 0, 231, 71));
        pushButton->setStyleSheet(QString::fromUtf8("QPushButton{font-size:15px;\n"
"color:blue;\n"
"}"));
        stackedWidget->addWidget(insertPowderPage);
        cottonsDescPage = new QWidget();
        cottonsDescPage->setObjectName("cottonsDescPage");
        title = new QLabel(cottonsDescPage);
        title->setObjectName("title");
        title->setGeometry(QRect(0, 0, 181, 51));
        title->setStyleSheet(QString::fromUtf8("QLabel {\n"
"font-size:20px;\n"
"font-weight:bold;\n"
"}"));
        textEdit = new QTextEdit(cottonsDescPage);
        textEdit->setObjectName("textEdit");
        textEdit->setGeometry(QRect(0, 50, 221, 171));
        temperatureBox = new QComboBox(cottonsDescPage);
        temperatureBox->addItem(QString());
        temperatureBox->addItem(QString());
        temperatureBox->addItem(QString());
        temperatureBox->addItem(QString());
        temperatureBox->addItem(QString());
        temperatureBox->setObjectName("temperatureBox");
        temperatureBox->setGeometry(QRect(120, 230, 91, 31));
        temperatureBox->setStyleSheet(QString::fromUtf8("QComboBox{\n"
"font-size:15px;\n"
"}"));
        temperatureBox->setEditable(false);
        tempLabel = new QLabel(cottonsDescPage);
        tempLabel->setObjectName("tempLabel");
        tempLabel->setGeometry(QRect(0, 230, 131, 31));
        tempLabel->setStyleSheet(QString::fromUtf8("QLabel{\n"
"font-size:20px;\n"
"}"));
        spinBox = new QComboBox(cottonsDescPage);
        spinBox->addItem(QString());
        spinBox->addItem(QString());
        spinBox->addItem(QString());
        spinBox->addItem(QString());
        spinBox->addItem(QString());
        spinBox->setObjectName("spinBox");
        spinBox->setGeometry(QRect(120, 270, 91, 31));
        spinBox->setStyleSheet(QString::fromUtf8("QComboBox{\n"
"font-size:15px;\n"
"}"));
        spinBox->setEditable(false);
        spinLabel = new QLabel(cottonsDescPage);
        spinLabel->setObjectName("spinLabel");
        spinLabel->setGeometry(QRect(40, 270, 131, 31));
        spinLabel->setStyleSheet(QString::fromUtf8("QLabel{\n"
"font-size:20px;\n"
"}"));
        rinseBox = new QComboBox(cottonsDescPage);
        rinseBox->addItem(QString());
        rinseBox->addItem(QString());
        rinseBox->addItem(QString());
        rinseBox->addItem(QString());
        rinseBox->setObjectName("rinseBox");
        rinseBox->setGeometry(QRect(120, 310, 91, 31));
        rinseBox->setStyleSheet(QString::fromUtf8("QComboBox{\n"
"font-size:15px;\n"
"}"));
        rinseBox->setEditable(false);
        rinseLabel = new QLabel(cottonsDescPage);
        rinseLabel->setObjectName("rinseLabel");
        rinseLabel->setGeometry(QRect(0, 310, 131, 31));
        rinseLabel->setStyleSheet(QString::fromUtf8("QLabel{\n"
"font-size:20px;\n"
"}"));
        ecomodeLabel = new QLabel(cottonsDescPage);
        ecomodeLabel->setObjectName("ecomodeLabel");
        ecomodeLabel->setGeometry(QRect(10, 350, 131, 31));
        ecomodeLabel->setStyleSheet(QString::fromUtf8("QLabel{\n"
"font-size:20px;\n"
"}"));
        ecomodeBox = new QComboBox(cottonsDescPage);
        ecomodeBox->addItem(QString());
        ecomodeBox->addItem(QString());
        ecomodeBox->setObjectName("ecomodeBox");
        ecomodeBox->setGeometry(QRect(120, 350, 91, 31));
        ecomodeBox->setStyleSheet(QString::fromUtf8("QComboBox{\n"
"font-size:15px;\n"
"}"));
        ecomodeBox->setEditable(false);
        startButton = new QPushButton(cottonsDescPage);
        startButton->setObjectName("startButton");
        startButton->setGeometry(QRect(0, 490, 101, 51));
        startButton->setStyleSheet(QString::fromUtf8("QPushButton{\n"
"font-size:20px;\n"
"padding:5px;\n"
"}"));
        goBackButton = new QPushButton(cottonsDescPage);
        goBackButton->setObjectName("goBackButton");
        goBackButton->setGeometry(QRect(120, 490, 101, 51));
        goBackButton->setStyleSheet(QString::fromUtf8("QPushButton{\n"
"font-size:20px;\n"
"padding:5px;\n"
"}"));
        hourLabel = new QLabel(cottonsDescPage);
        hourLabel->setObjectName("hourLabel");
        hourLabel->setGeometry(QRect(30, 400, 71, 41));
        hourLabel->setStyleSheet(QString::fromUtf8("QLabel {\n"
"font-size:20px;\n"
"border:2px solid black;\n"
"padding:6px;\n"
"}"));
        label_2 = new QLabel(cottonsDescPage);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(110, 410, 81, 31));
        label_2->setStyleSheet(QString::fromUtf8("QLabel{\n"
"font-size:20px;\n"
"}"));
        stackedWidget->addWidget(cottonsDescPage);
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1198, 22));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        stackedWidget->setCurrentIndex(2);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        cottonsButton->setText(QCoreApplication::translate("MainWindow", "Cottons", nullptr));
        cottonsEcoButton->setText(QCoreApplication::translate("MainWindow", "Cottons Eco", nullptr));
        syntheticsButton->setText(QCoreApplication::translate("MainWindow", "Synthetics", nullptr));
        woolSilkButton->setText(QCoreApplication::translate("MainWindow", "Wool/Silk", nullptr));
        antiAllergyButton->setText(QCoreApplication::translate("MainWindow", "Anti-Allergy", nullptr));
        nonStopButton->setText(QCoreApplication::translate("MainWindow", "NonStop 3h/3kg", nullptr));
        antiCreaseButton->setText(QCoreApplication::translate("MainWindow", "AntiCrease", nullptr));
        refreshButton->setText(QCoreApplication::translate("MainWindow", "Refresh", nullptr));
        pushButton->setText(QCoreApplication::translate("MainWindow", "Insert Powder", nullptr));
        title->setText(QCoreApplication::translate("MainWindow", "Cottons Program", nullptr));
        textEdit->setHtml(QCoreApplication::translate("MainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><meta charset=\"utf-8\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"hr { height: 1px; border-width: 0; }\n"
"li.unchecked::marker { content: \"\\2610\"; }\n"
"li.checked::marker { content: \"\\2612\"; }\n"
"</style></head><body style=\" font-family:'Segoe UI'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-weight:700;\">Load Capacity</span>: Up to <span style=\" font-weight:700;\">8 kg</span></p>\n"
"<p style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-weight:700;\">Water Temperature</span>: 20<span style=\" font-weight:700;\">\302\260C \342\200\223 60\302\260C</span></p>\n"
"<p style=\" margi"
                        "n-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-weight:700;\">Spin Speed</span>: <span style=\" font-weight:700;\">1400 RPM</span> (adjustable)</p>\n"
"<p style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-weight:700;\">Cycle Duration</span>: <span style=\" font-weight:700;\">2 hours</span> (approx., depending on options)</p>\n"
"<p style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-weight:700;\">Purpose</span>: For durable cotton fabrics like <span style=\" font-weight:700;\">t-shirts</span>, <span style=\" font-weight:700;\">sheets</span>, <span style=\" font-weight:700;\">towels</span>, <span style=\" font-weight:700;\">jeans</span>.</p></body></html>", nullptr));
        temperatureBox->setItemText(0, QCoreApplication::translate("MainWindow", "60\302\260C", nullptr));
        temperatureBox->setItemText(1, QCoreApplication::translate("MainWindow", "90\302\260C", nullptr));
        temperatureBox->setItemText(2, QCoreApplication::translate("MainWindow", "20\302\260C", nullptr));
        temperatureBox->setItemText(3, QCoreApplication::translate("MainWindow", "30\302\260C", nullptr));
        temperatureBox->setItemText(4, QCoreApplication::translate("MainWindow", "40\302\260C", nullptr));

        tempLabel->setText(QCoreApplication::translate("MainWindow", "Temperature:", nullptr));
        spinBox->setItemText(0, QCoreApplication::translate("MainWindow", "1200RPM", nullptr));
        spinBox->setItemText(1, QCoreApplication::translate("MainWindow", "600RPM", nullptr));
        spinBox->setItemText(2, QCoreApplication::translate("MainWindow", "800RPM", nullptr));
        spinBox->setItemText(3, QCoreApplication::translate("MainWindow", "1000RPM", nullptr));
        spinBox->setItemText(4, QCoreApplication::translate("MainWindow", "1400RPM", nullptr));

        spinLabel->setText(QCoreApplication::translate("MainWindow", "Spin:", nullptr));
        rinseBox->setItemText(0, QCoreApplication::translate("MainWindow", "2", nullptr));
        rinseBox->setItemText(1, QCoreApplication::translate("MainWindow", "1", nullptr));
        rinseBox->setItemText(2, QCoreApplication::translate("MainWindow", "3", nullptr));
        rinseBox->setItemText(3, QString());

        rinseLabel->setText(QCoreApplication::translate("MainWindow", "Rinse Cycles:", nullptr));
        ecomodeLabel->setText(QCoreApplication::translate("MainWindow", "Eco Mode:", nullptr));
        ecomodeBox->setItemText(0, QCoreApplication::translate("MainWindow", "Off", nullptr));
        ecomodeBox->setItemText(1, QCoreApplication::translate("MainWindow", "On", nullptr));

        startButton->setText(QCoreApplication::translate("MainWindow", "START", nullptr));
        goBackButton->setText(QCoreApplication::translate("MainWindow", "GO BACK", nullptr));
        hourLabel->setText(QCoreApplication::translate("MainWindow", "2", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "hours", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
