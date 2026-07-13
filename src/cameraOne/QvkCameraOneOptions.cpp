#include "QvkCameraOneOptions.h"
#include "ui_QvkCameraOneOptions.h"

QvkCameraOneOptions::QvkCameraOneOptions(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QvkCameraOneOptions)
{
    ui->setupUi(this);

    connect(ui->toolButtonCameraOneFramelessOnOff,
            &QToolButton::toggled,
            this, [=](bool value){
        if (value == false){
            ui->toolButtonCameraOneFramelessOnOff->setIcon(QIcon(":/pictures/camera/window-frameless-off.png"));
        }else{
            ui->toolButtonCameraOneFramelessOnOff->setIcon(QIcon(":/pictures/camera/window-frameless-on.png"));
        }
    });
}

QvkCameraOneOptions::~QvkCameraOneOptions()
{
    delete ui;
}
