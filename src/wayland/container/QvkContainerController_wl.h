#ifndef QVKCONTAINERCONTROLLER_WL_H
#define QVKCONTAINERCONTROLLER_WL_H

#include <QObject>

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
    void set_available_muxer_in_ComboBox();
    void set_videoencoder_to_available_or_unavailable();
    void set_audioencoder_to_available_or_unavailable();
    void set_available_formatVideoAudoicodec_in_tab();


private slots:
    void slot_set_available_VideoCodecs_in_Combobox( const QString suffix );
    void slot_set_available_AudioCodecs_in_Combobox( const QString suffix );


signals:


};

#endif // QVKCONTAINERCONTROLLER_H

