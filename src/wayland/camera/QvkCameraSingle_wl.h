#ifndef QVKCAMERASINGLE_WL_H
#define QVKCAMERASINGLE_WL_H

#include <QWidget>
#include <QCheckBox>

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
    void slot_checkBoxCameraOnOff(bool value, QCheckBox *checkBox);


private:


signals:
    void signal_forSystrayCameraOnOff(QCheckBox *checkBox);

};

#endif // QVKCAMERASINGLE_WL_H
