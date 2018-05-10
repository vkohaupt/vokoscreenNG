/********************************************************************************
** Form generated from reading UI file 'settings.ui'
**
** Created by: Qt User Interface Compiler version 5.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SETTINGS_H
#define UI_SETTINGS_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_AreaSettingsDialog
{
public:
    QHBoxLayout *horizontalLayout_2;
    QVBoxLayout *verticalLayout;
    QGridLayout *gridLayout;
    QSpinBox *area_X_SpinBox;
    QSpinBox *area_Y_SpinBox;
    QHBoxLayout *horizontalLayout_4;
    QSpacerItem *horizontalSpacer_8;
    QLabel *areaLabelY;
    QSpacerItem *horizontalSpacer_7;
    QHBoxLayout *horizontalLayout_7;
    QSpacerItem *horizontalSpacer_12;
    QLabel *areaLabelX;
    QSpacerItem *horizontalSpacer_10;
    QSpacerItem *verticalSpacer;
    QHBoxLayout *horizontalLayout_5;
    QSpacerItem *horizontalSpacer_5;
    QLabel *areaLabelHeight;
    QSpacerItem *horizontalSpacer_6;
    QHBoxLayout *horizontalLayout_3;
    QSpacerItem *horizontalSpacer_3;
    QLabel *areaLabelWidth;
    QSpacerItem *horizontalSpacer_9;
    QSpinBox *area_Height_SpinBox;
    QSpinBox *area_Width_SpinBox;
    QSpacerItem *horizontalSpacer;
    QSpacerItem *verticalSpacer_2;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *area_OK_Button;
    QPushButton *area_Cancel_Button;

    void setupUi(QDialog *AreaSettingsDialog)
    {
        if (AreaSettingsDialog->objectName().isEmpty())
            AreaSettingsDialog->setObjectName(QStringLiteral("AreaSettingsDialog"));
        AreaSettingsDialog->resize(314, 168);
        horizontalLayout_2 = new QHBoxLayout(AreaSettingsDialog);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        area_X_SpinBox = new QSpinBox(AreaSettingsDialog);
        area_X_SpinBox->setObjectName(QStringLiteral("area_X_SpinBox"));
        area_X_SpinBox->setMaximum(65535);

        gridLayout->addWidget(area_X_SpinBox, 1, 1, 1, 1);

        area_Y_SpinBox = new QSpinBox(AreaSettingsDialog);
        area_Y_SpinBox->setObjectName(QStringLiteral("area_Y_SpinBox"));
        area_Y_SpinBox->setMaximum(65535);

        gridLayout->addWidget(area_Y_SpinBox, 2, 1, 1, 1);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        horizontalSpacer_8 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_8);

        areaLabelY = new QLabel(AreaSettingsDialog);
        areaLabelY->setObjectName(QStringLiteral("areaLabelY"));
        areaLabelY->setText(QStringLiteral(""));

        horizontalLayout_4->addWidget(areaLabelY);

        horizontalSpacer_7 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_7);


        gridLayout->addLayout(horizontalLayout_4, 2, 0, 1, 1);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setObjectName(QStringLiteral("horizontalLayout_7"));
        horizontalSpacer_12 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_7->addItem(horizontalSpacer_12);

        areaLabelX = new QLabel(AreaSettingsDialog);
        areaLabelX->setObjectName(QStringLiteral("areaLabelX"));
        areaLabelX->setText(QStringLiteral(""));

        horizontalLayout_7->addWidget(areaLabelX);

        horizontalSpacer_10 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_7->addItem(horizontalSpacer_10);


        gridLayout->addLayout(horizontalLayout_7, 1, 0, 1, 1);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer, 0, 0, 1, 1);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_5);

        areaLabelHeight = new QLabel(AreaSettingsDialog);
        areaLabelHeight->setObjectName(QStringLiteral("areaLabelHeight"));
        areaLabelHeight->setText(QStringLiteral(""));
        areaLabelHeight->setScaledContents(false);

        horizontalLayout_5->addWidget(areaLabelHeight);

        horizontalSpacer_6 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_6);


        gridLayout->addLayout(horizontalLayout_5, 2, 3, 1, 1);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_3);

        areaLabelWidth = new QLabel(AreaSettingsDialog);
        areaLabelWidth->setObjectName(QStringLiteral("areaLabelWidth"));
        areaLabelWidth->setText(QStringLiteral(""));

        horizontalLayout_3->addWidget(areaLabelWidth);

        horizontalSpacer_9 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_9);


        gridLayout->addLayout(horizontalLayout_3, 1, 3, 1, 1);

        area_Height_SpinBox = new QSpinBox(AreaSettingsDialog);
        area_Height_SpinBox->setObjectName(QStringLiteral("area_Height_SpinBox"));
        area_Height_SpinBox->setMaximum(65535);

        gridLayout->addWidget(area_Height_SpinBox, 2, 4, 1, 1);

        area_Width_SpinBox = new QSpinBox(AreaSettingsDialog);
        area_Width_SpinBox->setObjectName(QStringLiteral("area_Width_SpinBox"));
        area_Width_SpinBox->setMaximum(65535);

        gridLayout->addWidget(area_Width_SpinBox, 1, 4, 1, 1);

        horizontalSpacer = new QSpacerItem(100, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 1, 2, 1, 1);


        verticalLayout->addLayout(gridLayout);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_2);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);

        area_OK_Button = new QPushButton(AreaSettingsDialog);
        area_OK_Button->setObjectName(QStringLiteral("area_OK_Button"));

        horizontalLayout->addWidget(area_OK_Button);

        area_Cancel_Button = new QPushButton(AreaSettingsDialog);
        area_Cancel_Button->setObjectName(QStringLiteral("area_Cancel_Button"));

        horizontalLayout->addWidget(area_Cancel_Button);


        verticalLayout->addLayout(horizontalLayout);


        horizontalLayout_2->addLayout(verticalLayout);

        QWidget::setTabOrder(area_X_SpinBox, area_Y_SpinBox);
        QWidget::setTabOrder(area_Y_SpinBox, area_Width_SpinBox);
        QWidget::setTabOrder(area_Width_SpinBox, area_Height_SpinBox);
        QWidget::setTabOrder(area_Height_SpinBox, area_OK_Button);
        QWidget::setTabOrder(area_OK_Button, area_Cancel_Button);

        retranslateUi(AreaSettingsDialog);

        QMetaObject::connectSlotsByName(AreaSettingsDialog);
    } // setupUi

    void retranslateUi(QDialog *AreaSettingsDialog)
    {
        AreaSettingsDialog->setWindowTitle(QApplication::translate("AreaSettingsDialog", "Area Settings", 0));
        area_OK_Button->setText(QApplication::translate("AreaSettingsDialog", "OK", 0));
        area_Cancel_Button->setText(QApplication::translate("AreaSettingsDialog", "Cancel", 0));
    } // retranslateUi

};

namespace Ui {
    class AreaSettingsDialog: public Ui_AreaSettingsDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SETTINGS_H
