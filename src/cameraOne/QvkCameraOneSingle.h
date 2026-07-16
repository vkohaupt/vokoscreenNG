#ifndef QVKCAMERAONESINGLE_H
#define QVKCAMERAONESINGLE_H

#include <QString>
#include <QWidget>
#include <QCheckBox>
#include <QResizeEvent>

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
    QvkCameraOneOptions *vkCameraOneOptions = NULL;

    Ui::QvkCameraOneSingle *ui;

    int cameraOneWindow_X = 200;
    int cameraOneWindow_Y = 200;

    void set_pixelformat_into_comboBox(QString device);
    void set_resolution_into_comboBox(QString device);


private:
    Ui_formMainWindow *GuiUi;


protected:
    void resizeEvent(QResizeEvent *event);

};

#endif // QVKCAMERAONESINGLE_H
