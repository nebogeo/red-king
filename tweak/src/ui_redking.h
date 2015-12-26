/********************************************************************************
** Form generated from reading UI file 'redkingfK4886.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef REDKINGFK4886_H
#define REDKINGFK4886_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QMainWindow>
#include <QtGui/QPushButton>
#include <QtGui/QStatusBar>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout;
    QVBoxLayout *host_params_holder;
    QLabel *label_4;
    QVBoxLayout *parasite_params_holder;
    QLabel *label_6;
    QPushButton *pushButton;
    QWidget *verticalLayoutWidget_2;
    QVBoxLayout *canvas_holder;
    QLabel *label_3;
    QWidget *verticalLayoutWidget_3;
    QVBoxLayout *parasite_cost_holder;
    QLabel *label_2;
    QWidget *verticalLayoutWidget_4;
    QVBoxLayout *host_cost_holder;
    QLabel *label;
    QWidget *verticalLayoutWidget_5;
    QVBoxLayout *matrix_holder;
    QLabel *label_5;
    QCheckBox *checkBox;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(800, 661);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        verticalLayoutWidget = new QWidget(centralwidget);
        verticalLayoutWidget->setObjectName(QString::fromUtf8("verticalLayoutWidget"));
        verticalLayoutWidget->setGeometry(QRect(0, 0, 331, 591));
        verticalLayout = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        host_params_holder = new QVBoxLayout();
        host_params_holder->setObjectName(QString::fromUtf8("host_params_holder"));
        label_4 = new QLabel(verticalLayoutWidget);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label_4->sizePolicy().hasHeightForWidth());
        label_4->setSizePolicy(sizePolicy);

        host_params_holder->addWidget(label_4);


        verticalLayout->addLayout(host_params_holder);

        parasite_params_holder = new QVBoxLayout();
        parasite_params_holder->setObjectName(QString::fromUtf8("parasite_params_holder"));
        label_6 = new QLabel(verticalLayoutWidget);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        sizePolicy.setHeightForWidth(label_6->sizePolicy().hasHeightForWidth());
        label_6->setSizePolicy(sizePolicy);

        parasite_params_holder->addWidget(label_6);


        verticalLayout->addLayout(parasite_params_holder);

        pushButton = new QPushButton(verticalLayoutWidget);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));

        verticalLayout->addWidget(pushButton);

        verticalLayoutWidget_2 = new QWidget(centralwidget);
        verticalLayoutWidget_2->setObjectName(QString::fromUtf8("verticalLayoutWidget_2"));
        verticalLayoutWidget_2->setGeometry(QRect(480, 0, 311, 591));
        canvas_holder = new QVBoxLayout(verticalLayoutWidget_2);
        canvas_holder->setObjectName(QString::fromUtf8("canvas_holder"));
        canvas_holder->setContentsMargins(0, 0, 0, 0);
        label_3 = new QLabel(verticalLayoutWidget_2);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        sizePolicy.setHeightForWidth(label_3->sizePolicy().hasHeightForWidth());
        label_3->setSizePolicy(sizePolicy);

        canvas_holder->addWidget(label_3);

        verticalLayoutWidget_3 = new QWidget(centralwidget);
        verticalLayoutWidget_3->setObjectName(QString::fromUtf8("verticalLayoutWidget_3"));
        verticalLayoutWidget_3->setGeometry(QRect(330, 150, 151, 151));
        parasite_cost_holder = new QVBoxLayout(verticalLayoutWidget_3);
        parasite_cost_holder->setObjectName(QString::fromUtf8("parasite_cost_holder"));
        parasite_cost_holder->setContentsMargins(0, 0, 0, 0);
        label_2 = new QLabel(verticalLayoutWidget_3);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        sizePolicy.setHeightForWidth(label_2->sizePolicy().hasHeightForWidth());
        label_2->setSizePolicy(sizePolicy);

        parasite_cost_holder->addWidget(label_2);

        verticalLayoutWidget_4 = new QWidget(centralwidget);
        verticalLayoutWidget_4->setObjectName(QString::fromUtf8("verticalLayoutWidget_4"));
        verticalLayoutWidget_4->setGeometry(QRect(330, 0, 151, 151));
        host_cost_holder = new QVBoxLayout(verticalLayoutWidget_4);
        host_cost_holder->setObjectName(QString::fromUtf8("host_cost_holder"));
        host_cost_holder->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(verticalLayoutWidget_4);
        label->setObjectName(QString::fromUtf8("label"));
        sizePolicy.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy);

        host_cost_holder->addWidget(label);

        verticalLayoutWidget_5 = new QWidget(centralwidget);
        verticalLayoutWidget_5->setObjectName(QString::fromUtf8("verticalLayoutWidget_5"));
        verticalLayoutWidget_5->setGeometry(QRect(330, 300, 151, 291));
        matrix_holder = new QVBoxLayout(verticalLayoutWidget_5);
        matrix_holder->setObjectName(QString::fromUtf8("matrix_holder"));
        matrix_holder->setContentsMargins(0, 0, 0, 0);
        label_5 = new QLabel(verticalLayoutWidget_5);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        sizePolicy.setHeightForWidth(label_5->sizePolicy().hasHeightForWidth());
        label_5->setSizePolicy(sizePolicy);

        matrix_holder->addWidget(label_5);

        checkBox = new QCheckBox(centralwidget);
        checkBox->setObjectName(QString::fromUtf8("checkBox"));
        checkBox->setGeometry(QRect(10, 600, 97, 22));
        MainWindow->setCentralWidget(centralwidget);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);
        QObject::connect(pushButton, SIGNAL(released()), MainWindow, SLOT(slot_restart()));
        QObject::connect(checkBox, SIGNAL(toggled(bool)), MainWindow, SLOT(slot_model(bool)));

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "Red King 0.1", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("MainWindow", "Host cost params", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("MainWindow", "Parasite cost params", 0, QApplication::UnicodeUTF8));
        pushButton->setText(QApplication::translate("MainWindow", "Reset simulation", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("MainWindow", "Simulation", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("MainWindow", "Parasite cost function", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("MainWindow", "Host cost function", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("MainWindow", "Interaction matrix", 0, QApplication::UnicodeUTF8));
        checkBox->setText(QApplication::translate("MainWindow", "Model D", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // REDKINGFK4886_H
