#ifndef QVKCAMERASINGLE_WL_H
#define QVKCAMERASINGLE_WL_H

#include "QvkCameraSurface_wl.h"

#include "ui_formMainWindow_wl.h"

#include <QString>
#include <QWidget>
#include <QCheckBox>
#include <QVBoxLayout>
#include <QCamera>
#include <QResizeEvent>
#include <QTimer>

namespace Ui {
class QvkCameraSingle_wl;
}

class QvkCameraSingle_wl : public QWidget
{
    Q_OBJECT

public:
    explicit QvkCameraSingle_wl(QWidget *parent = nullptr);
    ~QvkCameraSingle_wl();
    Ui::QvkCameraSingle_wl *ui;
    QvkCameraSurface_wl *vkCameraSurface_wl = NULL;
    void set_init(QString device);
    void set_GUIui(Ui_formMainWindow_wl *ui);
    void set_PixelFormatOnComboBox(QString ID);
    void set_ResolutionOnComboBox(QString ID);


private:
    QCamera *camera = NULL;
    Ui_formMainWindow_wl *GuiUi;
    QTimer *timerNoImage;

private slots:
    void slot_checkBoxCameraOnOff(bool checked, QCheckBox *checkBoxCameraOnOff);


signals:
    void signal_forSystrayCameraOnOff(QCheckBox *checkBox);
    void signal_checkBoxCameraOnOff(bool value);
    void signal_cameraImageAvailable();


protected:
    void resizeEvent(QResizeEvent *event);

};

#endif // QVKCAMERASINGLE_WL_H
