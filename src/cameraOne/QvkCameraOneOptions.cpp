#include "QvkCameraOneOptions.h"
#include "ui_QvkCameraOneOptions.h"

QvkCameraOneOptions::QvkCameraOneOptions(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QvkCameraOneOptions)
{
    ui->setupUi(this);

    connect(ui->toolButtonCameraFramelessOnOff,
            &QToolButton::toggled,
            this, [=](bool value){
        if (value == false){
            ui->toolButtonCameraFramelessOnOff->setIcon(QIcon(":/pictures/camera/window-frameless-off.png"));
        }else{
            ui->toolButtonCameraFramelessOnOff->setIcon(QIcon(":/pictures/camera/window-frameless-on.png"));
        }
    });
}

QvkCameraOneOptions::~QvkCameraOneOptions()
{
    delete ui;
}
