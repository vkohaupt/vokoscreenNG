#ifndef QVKAUDIOPIPEWIRESINGLE_WL_H
#define QVKAUDIOPIPEWIRESINGLE_WL_H

#include <QFrame>

#include "ui_formMainWindow_wl.h"

namespace Ui {
class QvkAudioPipewireSingle_wl;
}

class QvkAudioPipewireSingle_wl : public QFrame
{
    Q_OBJECT

public:
    explicit QvkAudioPipewireSingle_wl(QWidget *parent = nullptr);
    ~QvkAudioPipewireSingle_wl();
    void init(QString string);
    void set_GUIui(Ui_formMainWindow_wl *ui);


public slots:


private:
    Ui::QvkAudioPipewireSingle_wl *ui;
    Ui_formMainWindow_wl *GuiUi;


signals:
    void signal_haveAudioDeviceSelected(bool);

};

#endif // QVKAUDIOPIPEWIRESINGLE_WL_H
