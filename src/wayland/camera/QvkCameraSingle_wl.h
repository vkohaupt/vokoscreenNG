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
    void set_objectName(QString device);
    void set_GUIui(Ui_formMainWindow_wl *ui);


private:
    QCamera *camera = NULL;
    QvkCameraSurface_wl *vkCameraSurface_wl = NULL;
    Ui_formMainWindow_wl *GuiUi;


private slots:
    void slot_checkBoxCameraOnOff(bool checked, QCheckBox *checkBoxCameraOnOff);


signals:
    void signal_forSystrayCameraOnOff(QCheckBox *checkBox);
    void signal_checkBoxCameraOnOff(bool value);


protected:
    void resizeEvent(QResizeEvent *event);

};

#endif // QVKCAMERASINGLE_WL_H
