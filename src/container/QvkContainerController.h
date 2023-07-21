#ifndef QVKCONTAINERCONTROLLER_H
#define QVKCONTAINERCONTROLLER_H

#include <QObject>

#include "ui_formMainWindow.h"
#include "QvkContainer.h"

class QvkContainerController : public QObject
{
    Q_OBJECT

public:
    QvkContainerController( Ui::formMainWindow *Ui );
    void init( bool isFlatpak );

    
private:
    Ui::formMainWindow *ui;
    QvkContainer *vkContainer;
    void set_muxer_to_available_or_unavailable();
    void set_available_muxer_in_ComboBox();
    void set_videoencoder_to_available_or_unavailable();
    void set_audioencoder_to_available_or_unavailable();
    void set_available_formatVideoAudoicodec_in_tab();


private slots:
    void slot_set_available_VideoCodecs_in_Combobox( const QString suffix );
    void slot_set_available_AudioCodecs_in_Combobox( const QString suffix );


public slots:


signals:


};

#endif // QVKCONTAINERCONTROLLER_H

