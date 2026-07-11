#ifndef QVKCAMERAONESINGLE_H
#define QVKCAMERAONESINGLE_H

#include <QWidget>
#include <QCheckBox>

#include "QvkCameraOneWindow.h"
#include "ui_QvkCameraOneSingle.h"

namespace Ui {
class QvkCameraOneSingle;
}

class QvkCameraOneSingle : public QWidget
{
    Q_OBJECT

public:
    explicit QvkCameraOneSingle(QWidget *parent = nullptr, QString device = "");
    ~QvkCameraOneSingle();
    QvkCameraOneWindow *vkCameraOneWindow = NULL;
    int cameraOneWindow_X = 200;
    int cameraOneWindow_Y = 200;
    Ui::QvkCameraOneSingle *ui;


private:


};

#endif // QVKCAMERAONESINGLE_H
