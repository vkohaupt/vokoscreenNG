#ifndef QVKCONTAINERCONTROLLER_WL_H
#define QVKCONTAINERCONTROLLER_WL_H

#include <QObject>
#include <QString>

#include "ui_formMainWindow_wl.h"
#include "QvkContainer_wl.h"

class QvkContainerController_wl : public QObject
{
    Q_OBJECT

public:
    QvkContainerController_wl( Ui::formMainWindow_wl *Ui );

    
private:
    Ui::formMainWindow_wl *ui;
    QvkContainer_wl *vkContainer_wl;
    void set_muxer_to_available_or_unavailable();
    void set_videoencoder_to_available_or_unavailable();
    void set_audioencoder_to_available_or_unavailable();
    void set_available_formatVideoAudoicodec_in_tab();
    void set_available_AudioCodecs_in_Combobox();


private slots:


signals:


};

#endif // QVKCONTAINERCONTROLLER_H

