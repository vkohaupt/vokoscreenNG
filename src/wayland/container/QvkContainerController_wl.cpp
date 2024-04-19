#include "QvkContainerController_wl.h"
#include "global.h"

#include <QDebug>
#include <QLabel>
#include <QMessageBox>
#include <gst/gst.h>

QvkContainerController_wl::QvkContainerController_wl( Ui::formMainWindow_wl *Ui )
{
//    Q_UNUSED(parent)

    ui = Ui;
    vkContainer_wl = new QvkContainer_wl;

    set_muxer_to_available_or_unavailable();
    set_videoencoder_to_available_or_unavailable();
    set_audioencoder_to_available_or_unavailable();

    // GUI
    connect( ui->comboBoxFormat, SIGNAL( currentTextChanged(QString) ), this, SLOT( slot_set_available_VideoCodecs_in_Combobox(QString) ) );
    connect( ui->comboBoxFormat, SIGNAL( currentTextChanged(QString) ), this, SLOT( slot_set_available_AudioCodecs_in_Combobox(QString) ) );

    set_available_muxer_in_ComboBox();
    set_available_formatVideoAudoicodec_in_tab();

    qDebug();
    qDebug().noquote() << global::nameOutput << "Video encoder:";
    QStringList videoList = vkContainer_wl->get_AllVideoEncoders();
    for ( int i = 0; i < videoList.count(); i++ ) {
        QString encoder = videoList.at(i);
        GstElementFactory *factory = gst_element_factory_find( encoder.toLatin1() );
        if ( !factory ) {
            qDebug().noquote() << global::nameOutput << "-" << encoder;
        } else {
            qDebug().noquote() << global::nameOutput << "+" << encoder;
            gst_object_unref( factory );
        }
    }

    qDebug();
    qDebug().noquote() << global::nameOutput << "Audio encoder:";
    QStringList audioList = vkContainer_wl->get_AllAudioEncoders();
    for ( int i = 0; i < audioList.count(); i++ ) {
        QString encoder = audioList.at(i);
        GstElementFactory *factory = gst_element_factory_find( encoder.toLatin1() );
        if ( !factory ) {
            qDebug().noquote() << global::nameOutput << "-" << encoder;
        } else {
            qDebug().noquote() << global::nameOutput << "+" << encoder;
            gst_object_unref( factory );
        }
    }
}


void QvkContainerController_wl::set_muxer_to_available_or_unavailable()
{
    qDebug().noquote() << global::nameOutput << "Formats, video- and audio codec are only for record, not for the player";
    qDebug().noquote() << global::nameOutput << "Video formats:";
    for ( int i = 0; i < vkContainer_wl->get_Containers().count(); i++ )
    {
        QString muxer = vkContainer_wl->get_Containers().at(i)->get_Muxer();
        QString suffix = vkContainer_wl->get_Containers().at(i)->get_Suffix();
        GstElementFactory *factory = gst_element_factory_find( muxer.toLatin1() );
        if ( !factory ) {
            qDebug().noquote() << global::nameOutput << "-" << muxer << "(" + suffix + ")";
            vkContainer_wl->set_ContainerAvailable( muxer, false );
        } else {
            qDebug().noquote() << global::nameOutput <<  "+" << muxer << "(" + suffix + ")";
            vkContainer_wl->set_ContainerAvailable( muxer, true );
            gst_object_unref( factory );
        }
    }
}


void QvkContainerController_wl::set_videoencoder_to_available_or_unavailable()
{
    for ( int i = 0; i < vkContainer_wl->get_Containers().count(); i++ )
    {
        QString suffix = vkContainer_wl->get_Containers().at(i)->get_Suffix();
        QList<Container_wl::VideoCodec_wl> list = vkContainer_wl->get_VideoCodecs( suffix  );
        if ( !list.empty() ) {
            for ( int x = 0; x < list.count(); x++ ) {
                QString encoder = list.at(x).encoder;
                GstElementFactory *factory = gst_element_factory_find( encoder.toLatin1() );
                if ( !factory ) {
                    vkContainer_wl->get_Containers().at(i)->set_VideoCodecAvailable( encoder, false );
                } else {
                    vkContainer_wl->get_Containers().at(i)->set_VideoCodecAvailable( encoder, true );
                    gst_object_unref( factory );
                }
            }
        }
    }
}


void QvkContainerController_wl::set_audioencoder_to_available_or_unavailable()
{
    for ( int i = 0; i < vkContainer_wl->get_Containers().count(); i++ )
    {
        QString suffix = vkContainer_wl->get_Containers().at(i)->get_Suffix();
        QList<Container_wl::AudioCodec_wl> list = vkContainer_wl->get_AudioCodecs( suffix  );
        if ( !list.empty() ) {
            for ( int x = 0; x < list.count(); x++ ) {
                QString encoder = list.at(x).encoder;
                GstElementFactory *factory = gst_element_factory_find( encoder.toLatin1() );
                if ( !factory ) {
                    vkContainer_wl->get_Containers().at(i)->set_AudioCodecAvailable( encoder, false );
                } else {
                    vkContainer_wl->get_Containers().at(i)->set_AudioCodecAvailable( encoder, true );
                    gst_object_unref( factory );
                }
            }
        }
    }
}


void QvkContainerController_wl::set_available_muxer_in_ComboBox()
{
    for ( int i = 0; i < vkContainer_wl->get_Containers().count(); i++ ) {
        if ( vkContainer_wl->get_Containers().at(i)->get_Available() == true ) {
            QIcon icon;
            if ( vkContainer_wl->get_Containers().at(i)->get_Suffix() == "mkv" ){
                icon = QIcon( ":/pictures/screencast/strip-mkv.png" );
            }
            if ( vkContainer_wl->get_Containers().at(i)->get_Suffix() == "avi" ){
                icon = QIcon( ":/pictures/screencast/strip-avi.png" );
            }
            if ( vkContainer_wl->get_Containers().at(i)->get_Suffix() == "webm" ){
                icon = QIcon( ":/pictures/screencast/strip-webm.png" );
            }
            if ( vkContainer_wl->get_Containers().at(i)->get_Suffix() == "mp4" ){
                icon = QIcon( ":/pictures/screencast/strip-mp4.png" );
            }
            if ( vkContainer_wl->get_Containers().at(i)->get_Suffix() == "mov" ){
                icon = QIcon( ":/pictures/screencast/strip-mov.png" );
            }
            ui->comboBoxFormat->addItem( icon, vkContainer_wl->get_Containers().at(i)->get_Suffix(), vkContainer_wl->get_Containers().at(i)->get_Muxer() );
        }
    }
}


/*
 * Insert Video encoder and Video name from container in Videocodec-Combobox
 */
void QvkContainerController_wl::slot_set_available_VideoCodecs_in_Combobox( const QString suffix )
{
    ui->comboBoxVideoCodec->clear();
    QList<Container_wl::VideoCodec_wl> list = vkContainer_wl->get_VideoCodecs( suffix  );
    if ( !list.empty() ) {
        qDebug();
        qDebug().noquote() << global::nameOutput << "Format was changed, codecs are inserted:";
        for ( int i = 0; i < list.count(); i++ ) {
            if ( list.at(i).available == true ) {
#ifdef Q_OS_WIN
                if ( ui->radioButton_cisco_off->isChecked() and ( list.at(i).encoder == "openh264enc" ) ) {
                    continue;
                }
#endif
                GstElementFactory *factory = gst_element_factory_find( list.at(i).encoder.toLatin1() );
                if ( !factory ) {
                    qDebug().noquote() << global::nameOutput << "-" << list.at(i).encoder;
                } else {
                    QString message = global::nameOutput + " + " + list.at(i).encoder;
                    GstElement *source = gst_element_factory_create( factory, "source" );
                    if ( !source ) {
                        message = global::nameOutput + " - " + list.at(i).encoder + " available but codec is missing";
                    } else {
                        ui->comboBoxVideoCodec->addItem( list.at(i).name, list.at(i).encoder );
                        gst_object_unref( source );
                    }

                    qDebug().noquote() << message;
                    gst_object_unref( factory );
                }
            }
        }

        if ( ui->comboBoxVideoCodec->count() == 0 )
        {
            QPixmap pixmap( ":/pictures/status/information.png" );
            pixmap = pixmap.scaled( 64, 64, Qt::IgnoreAspectRatio, Qt::SmoothTransformation );

            QMessageBox messageBox;
            messageBox.setIconPixmap( pixmap );
            messageBox.setText( "<b>No videocodec found</b>" );

            QString string;
            string += "<center>Please install package</center><br>";
            string += "gstreamer-plugins-base<br>";
            string += "gstreamer-plugins-good<br>";
            string += "gstreamer-plugins-bad<br>";
            string += "gstreamer-plugins-ugly<br>";
            string += "gstreamer-plugins-libav<br>";
            messageBox.setInformativeText( string );
            messageBox.exec();

            ui->pushButtonStart->setEnabled( false );
        }
        else
        {
            ui->pushButtonStart->setEnabled( true );
        }
    }
}


/*
 * Insert audio-encoder and video-name from container in audiocodec Combobox
 */
void QvkContainerController_wl::slot_set_available_AudioCodecs_in_Combobox( const QString suffix )
{
    ui->comboBoxAudioCodec->clear();
    QList<Container_wl::AudioCodec_wl> list = vkContainer_wl->get_AudioCodecs( suffix  );
    if ( !list.empty() ) {
        for ( int i = 0; i < list.count(); i++ ) {
            if ( list.at(i).available == true ) {
                ui->comboBoxAudioCodec->addItem( list.at(i).name, list.at(i).encoder );
            }
        }
    }
}


// Check format, video and audoicodec on tab availability
void QvkContainerController_wl::set_available_formatVideoAudoicodec_in_tab()
{
    // Delete all QLabel
    QList<QLabel *> listLabel = ui->scrollAreaWidgetContentsAvailable->findChildren<QLabel *>();
    for( int i = 0; i < listLabel.count(); i++ ) {
        delete listLabel.at( i );
    }

    // Delete spacerItem
    for ( int i = 0; i < ui->gridLayoutAvailable->count(); ++i ) {
        QLayoutItem *layoutItem = ui->gridLayoutAvailable->itemAt(i);
        if ( layoutItem->spacerItem() ) {
            ui->gridLayoutAvailable->removeItem(layoutItem);
            delete layoutItem;
            --i;
        }
    }

    // Delete line
    QList<QFrame *> listFrame = ui->scrollAreaWidgetContentsAvailable->findChildren<QFrame *>();
    for( int i = 0; i < listFrame.count(); i++ ) {
        delete listFrame.at( i );
    }

    // Adding all informations
    int rowCount = 0;
    for ( int i = 0; i < vkContainer_wl->get_Containers().count(); i++ )
    {
        int rowMuxer = 1;
        int rowVideo = 1;
        int rowAudio = 1;
        QIcon icon( QString::fromUtf8( ":/pictures/screencast/accept.png" ) );
        if ( vkContainer_wl->get_Containers().at(i)->get_Available() == false ) {
            icon.addFile( QString::fromUtf8( ":/pictures/screencast/missing.png" ) );
        }
        QSize size = icon.actualSize( QSize( 16, 16 ), QIcon::Normal, QIcon::On );
        QLabel *labelPicture = new QLabel();
        labelPicture->setPixmap( icon.pixmap( size, QIcon::Normal, QIcon::On ));
        labelPicture->setAlignment( Qt::AlignRight );
        ui->gridLayoutAvailable->addWidget( labelPicture, rowCount + rowMuxer, 0 );
        ui->gridLayoutAvailable->addWidget( new QLabel( "  " + QString( vkContainer_wl->get_Containers().at(i)->get_Suffix() ) ), rowCount + rowMuxer, 1 );
        rowMuxer++;

        QList<Container_wl::VideoCodec_wl> list_VideoCodecs = vkContainer_wl->get_VideoCodecs( vkContainer_wl->get_Containers().at(i)->get_Suffix()  );
        if ( !list_VideoCodecs.empty() )
        {
            for ( int i = 0; i < list_VideoCodecs.count(); i++ )
            {
                QIcon icon( QString::fromUtf8( ":/pictures/screencast/accept.png" ) );
                if ( list_VideoCodecs.at(i).available == false ) {
                    icon.addFile( QString::fromUtf8( ":/pictures/screencast/missing.png" ) );
                }
                QSize size = icon.actualSize( QSize( 16, 16 ), QIcon::Normal, QIcon::On );
                QLabel *labelPicture = new QLabel();
                labelPicture->setPixmap( icon.pixmap( size, QIcon::Normal, QIcon::On ));
                labelPicture->setAlignment( Qt::AlignRight );
                ui->gridLayoutAvailable->addWidget( labelPicture, rowCount + rowVideo, 2 );
                ui->gridLayoutAvailable->addWidget( new QLabel( "  " + QString( list_VideoCodecs.at(i).name ) ), rowCount + rowVideo, 3 );
                rowVideo++;
            }
        }

        QList<Container_wl::AudioCodec_wl> list_AudioCodecs = vkContainer_wl->get_AudioCodecs( vkContainer_wl->get_Containers().at(i)->get_Suffix()  );
        if ( !list_AudioCodecs.empty() )
        {
            for ( int i = 0; i < list_AudioCodecs.count(); i++ )
            {
                QIcon icon( QString::fromUtf8( ":/pictures/screencast/accept.png" ) );
                if ( list_AudioCodecs.at(i).available == false ) {
                    icon.addFile( QString::fromUtf8( ":/pictures/screencast/missing.png" ) );
                }
                QSize size = icon.actualSize( QSize( 16, 16 ), QIcon::Normal, QIcon::On );
                QLabel *labelPicture = new QLabel();
                labelPicture->setPixmap( icon.pixmap( size, QIcon::Normal, QIcon::On ));
                labelPicture->setAlignment( Qt::AlignRight );
                ui->gridLayoutAvailable->addWidget( labelPicture, rowCount + rowAudio, 4 );
                ui->gridLayoutAvailable->addWidget( new QLabel( "  " + QString( list_AudioCodecs.at(i).name ) ), rowCount + rowAudio, 5 );
                rowAudio++;
            }
        }

        rowCount = ui->gridLayoutAvailable->rowCount();
        for ( int x = 0; x <= 5; x++ )
        {
            QFrame *line = new QFrame();
            line->setObjectName( QStringLiteral( "line" ) );
            line->setFrameShape( QFrame::HLine );
            line->setFrameShadow( QFrame::Sunken );
            ui->gridLayoutAvailable->addWidget( line, rowCount, x );

            QLabel *label = new QLabel;
            label->setText( " " );
            ui->gridLayoutAvailable->addWidget( label, rowCount, x );
        }
    }

    ui->gridLayoutAvailable->addItem( new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding),
                                      ui->gridLayoutAvailable->rowCount() + 1, 0 );
}
