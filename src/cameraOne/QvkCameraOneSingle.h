#ifndef QVKCAMERAONESINGLE_H
#define QVKCAMERAONESINGLE_H

#include <QWidget>
#include <QCheckBox>

#include "QvkCameraOneWindow.h"
#include "QvkCameraOneOptions.h"

#include "ui_QvkCameraOneSingle.h"
#include "ui_formMainWindow.h"

namespace Ui {
class QvkCameraOneSingle;
}

class QvkCameraOneSingle : public QWidget
{
    Q_OBJECT

public:
    explicit QvkCameraOneSingle(QWidget *parent, QString device, Ui_formMainWindow *m_GuiUi);
    ~QvkCameraOneSingle();
    QvkCameraOneWindow *vkCameraOneWindow = NULL;
    int cameraOneWindow_X = 200;
    int cameraOneWindow_Y = 200;
    Ui::QvkCameraOneSingle *ui;
    QvkCameraOneOptions *vkCameraOneOptions = NULL;

private:
    Ui_formMainWindow *GuiUi;


};

#endif // QVKCAMERAONESINGLE_H
