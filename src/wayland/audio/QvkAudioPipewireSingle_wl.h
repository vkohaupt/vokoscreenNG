#ifndef QVKAUDIOPIPEWIRESINGLE_WL_H
#define QVKAUDIOPIPEWIRESINGLE_WL_H

#include <QFrame>

namespace Ui {
class QvkAudioPipewireSingle_wl;
}

class QvkAudioPipewireSingle_wl : public QFrame
{
    Q_OBJECT

public:
    explicit QvkAudioPipewireSingle_wl(QWidget *parent = nullptr);
    ~QvkAudioPipewireSingle_wl();

private:
    Ui::QvkAudioPipewireSingle_wl *ui;
};

#endif // QVKAUDIOPIPEWIRESINGLE_WL_H
