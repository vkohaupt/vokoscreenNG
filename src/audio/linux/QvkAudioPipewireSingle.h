#ifndef QVKAUDIOPIPEWIRESINGLE_H
#define QVKAUDIOPIPEWIRESINGLE_H

#include <QWidget>
#include <QFrame>
#include <QMouseEvent>

#include "QvkAudioPipewireLevelMeter.h"

#include "ui_formMainWindow.h"

namespace Ui {
class QvkAudioPipewireSingle;
}

class QvkAudioPipewireSingle : public QFrame
{
    Q_OBJECT

public:
    explicit QvkAudioPipewireSingle(QWidget *parent = nullptr);
    ~QvkAudioPipewireSingle();
    void init(QString string);
    void set_GUIui(Ui_formMainWindow *ui);
    QvkAudioPipewireLevelMeter *vkAudioPipewireLevelMeter = nullptr;


public slots:


private:
    Ui::QvkAudioPipewireSingle *ui;
    Ui_formMainWindow *GuiUi;


signals:
    void signal_haveAudioDeviceSelected(bool);


protected:
    void mouseReleaseEvent(QMouseEvent *event);

};

#endif // QVKAUDIOPIPEWIRESINGLE_H
