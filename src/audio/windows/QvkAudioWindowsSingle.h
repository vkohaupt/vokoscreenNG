#ifndef QVKAUDIOWINDOWSSINGLE_H
#define QVKAUDIOWINDOWSSINGLE_H

#include <QWidget>
#include <QFrame>
#include <QMouseEvent>

#include "QvkAudioWindowsLevelMeter.h"

#include "ui_formMainWindow.h"

namespace Ui {
class QvkAudioWindowsSingle;
}

class QvkAudioWindowsSingle : public QFrame
{
    Q_OBJECT

public:
    explicit QvkAudioWindowsSingle(QWidget *parent = nullptr);
    ~QvkAudioWindowsSingle();
    void init(QString string);
    void set_GUIui(Ui_formMainWindow *ui);
    QvkAudioWindowsLevelMeter *vkAudioWindowsLevelMeter = nullptr;


public slots:


private:
    Ui::QvkAudioWindowsSingle *ui;
    Ui_formMainWindow *GuiUi;


signals:
    void signal_haveAudioDeviceSelected(bool);


protected:
    void mouseReleaseEvent(QMouseEvent *event);

};

#endif // QVKAUDIOWINDOWSSINGLE_H
