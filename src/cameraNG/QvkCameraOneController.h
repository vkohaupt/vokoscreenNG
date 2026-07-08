#ifndef QVKCAMERAONECONTROLLER_H
#define QVKCAMERAONECONTROLLER_H

#include <QWidget>
#include <QCheckBox>

#include "QvkCameraNG.h"


class QvkCameraOneController : public QWidget
{
    Q_OBJECT

public:
    explicit QvkCameraOneController(QWidget *parent = nullptr);
    ~QvkCameraOneController();
    QvkCameraNG *vkCameraNG = NULL;
    int cameraNG_X = 200;
    int cameraNG_Y = 200;
    QCheckBox *checkBox_Camera_OnOff;
    QCheckBox *checkBox_Frame_OnOff;


private:


};

#endif // QVKCAMERAONECONTROLLER_H
