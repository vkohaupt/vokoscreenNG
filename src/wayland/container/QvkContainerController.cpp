#include "QvkContainerController.h"

#include <QDebug>
#include <QLabel>
#include <gst/gst.h>

QvkContainerController::QvkContainerController(QObject *parent, Ui::formMainWindow_wl *Ui )
{
    Q_UNUSED(parent)

    ui = Ui;
    vkContainer = new QvkContainer;

    set_muxer_to_available_or_unavailable();
    set_videoencoder_to_available_or_unavailable();
    set_audioencoder_to_available_or_unavailable();

    // GUI
    connect( ui->comboBoxFormat, SIGNAL( currentTextChanged( const QString ) ), this, SLOT( slot_set_available_VideoCodecs_in_Combobox( const QString ) ) );
    connect( ui->comboBoxFormat, SIGNAL( currentTextChanged( const QString ) ), this, SLOT( slot_set_available_AudioCodecs_in_Combobox( const QString ) ) );

    set_available_muxer_in_ComboBox();
    set_available_formatVideoAudoicodec_in_tab();
}


void QvkContainerController::set_muxer_to_available_or_unavailable()
{
    for ( int i = 0; i < vkContainer->get_Containers().count(); i++ )
    {
        QString muxer = vkContainer->get_Containers().at(i)->get_Muxer();
        GstElementFactory *factory = gst_element_factory_find( muxer.toLatin1() );
        if ( !factory )
        {
            vkContainer->set_ContainerAvailable( muxer, false );
        }
        else
        {
            vkContainer->set_ContainerAvailable( muxer, true );
            gst_object_unref( factory );
        }
    }
}


void QvkContainerController::set_videoencoder_to_available_or_unavailable()
{
    for ( int i = 0; i < vkContainer->get_Containers().count(); i++ )
    {
        QString suffix = vkContainer->get_Containers().at(i)->get_Suffix();
        QList<Container::VideoCodec> list = vkContainer->get_VideoCodecs( suffix  );
        if ( !list.empty() )
        {
            for ( int x = 0; x < list.count(); x++ )
            {
                QString encoder = list.at(x).encoder;
                GstElementFactory *factory = gst_element_factory_find( encoder.toLatin1() );
                if ( !factory )
                {
                    vkContainer->get_Containers().at(i)->set_VideoCodecAvailable( encoder, false );
                }
                else
                {
                    vkContainer->get_Containers().at(i)->set_VideoCodecAvailable( encoder, true );
                    gst_object_unref( factory );
                }
            }
        }
    }
}


void QvkContainerController::set_audioencoder_to_available_or_unavailable()
{
    for ( int i = 0; i < vkContainer->get_Containers().count(); i++ )
    {
        QString suffix = vkContainer->get_Containers().at(i)->get_Suffix();
        QList<Container::AudioCodec> list = vkContainer->get_AudioCodecs( suffix  );
        if ( !list.empty() )
        {
            for ( int x = 0; x < list.count(); x++ )
            {
                QString encoder = list.at(x).encoder;
                GstElementFactory *factory = gst_element_factory_find( encoder.toLatin1() );
                if ( !factory )
                {
                    vkContainer->get_Containers().at(i)->set_AudioCodecAvailable( encoder, false );
                }
                else
                {
                    vkContainer->get_Containers().at(i)->set_AudioCodecAvailable( encoder, true );
                    gst_object_unref( factory );
                }
            }
        }
    }
}



void QvkContainerController::set_available_muxer_in_ComboBox()
{
    for ( int i = 0; i < vkContainer->get_Containers().count(); i++ )
    {
        if ( vkContainer->get_Containers().at(i)->get_Available() == true )
        {
            ui->comboBoxFormat->addItem( vkContainer->get_Containers().at(i)->get_Suffix(), vkContainer->get_Containers().at(i)->get_Muxer() );
        }
    }
}


/*
 * Insert Video encoder and Video name from container in Videocodec-Combobox
 */
void QvkContainerController::slot_set_available_VideoCodecs_in_Combobox( const QString suffix )
{
    ui->comboBoxVideoCodec->clear();
    QList<Container::VideoCodec> list = vkContainer->get_VideoCodecs( suffix  );
    if ( !list.empty() )
    {
        for ( int i = 0; i < list.count(); i++ )
        {
            if ( list.at(i).available == true )
            {
                ui->comboBoxVideoCodec->addItem( list.at(i).name, list.at(i).encoder );
            }
        }
    }
}

/*
 * Insert audio-encoder and video-name from container in audiocodec Combobox
 */
void QvkContainerController::slot_set_available_AudioCodecs_in_Combobox( const QString suffix )
{
    ui->comboBoxAudioCodec->clear();
    QList<Container::AudioCodec> list = vkContainer->get_AudioCodecs( suffix  );
    if ( !list.empty() )
    {
        for ( int i = 0; i < list.count(); i++ )
        {
            if ( list.at(i).available == true )
            {
                ui->comboBoxAudioCodec->addItem( list.at(i).name, list.at(i).encoder );
            }
        }
    }
}


// Check format, video and audoicodec on tab availability
void QvkContainerController::set_available_formatVideoAudoicodec_in_tab()
{
    // Delete all QLabel
    QList<QLabel *> listLabel = ui->scrollAreaWidgetContentsAvailable->findChildren<QLabel *>();
    for( int i = 0; i < listLabel.count(); i++ )
    {
        delete listLabel.at( i );
    }

    // Delete spacerItem
    for ( int i = 0; i < ui->gridLayoutAvailable->count(); ++i )
    {
        QLayoutItem *layoutItem = ui->gridLayoutAvailable->itemAt(i);
        if ( layoutItem->spacerItem() )
        {
            ui->gridLayoutAvailable->removeItem(layoutItem);
            delete layoutItem;
            --i;
        }
    }

    // Delete line
    QList<QFrame *> listFrame = ui->scrollAreaWidgetContentsAvailable->findChildren<QFrame *>();
    for( int i = 0; i < listFrame.count(); i++ )
    {
        delete listFrame.at( i );
    }

    // Adding all informations
    int rowCount = 0;
    for ( int i = 0; i < vkContainer->get_Containers().count(); i++ )
    {
        int rowMuxer = 1;
        int rowVideo = 1;
        int rowAudio = 1;
        QIcon icon( QString::fromUtf8( ":/pictures/screencast/accept.png" ) );
        if ( vkContainer->get_Containers().at(i)->get_Available() == false )
        {
            icon.addFile( QString::fromUtf8( ":/pictures/screencast/missing.png" ) );
        }
        QSize size = icon.actualSize( QSize( 16, 16 ), QIcon::Normal, QIcon::On );
        QLabel *labelPicture = new QLabel();
        labelPicture->setPixmap( icon.pixmap( size, QIcon::Normal, QIcon::On ));
        labelPicture->setAlignment( Qt::AlignRight );
        ui->gridLayoutAvailable->addWidget( labelPicture, rowCount + rowMuxer, 0 );
        ui->gridLayoutAvailable->addWidget( new QLabel( "  " + QString( vkContainer->get_Containers().at(i)->get_Suffix() ) ), rowCount + rowMuxer, 1 );
        rowMuxer++;

        QList<Container::VideoCodec> list_VideoCodecs = vkContainer->get_VideoCodecs( vkContainer->get_Containers().at(i)->get_Suffix()  );
        if ( !list_VideoCodecs.empty() )
        {
            for ( int i = 0; i < list_VideoCodecs.count(); i++ )
            {
                QIcon icon( QString::fromUtf8( "/home/vk/Programmieren/QList-Codec/codec/accept.png" ) );
                if ( list_VideoCodecs.at(i).available == false )
                {
                    icon.addFile( QString::fromUtf8( "/home/vk/Programmieren/QList-Codec/codec/missing.png" ) );
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

        QList<Container::AudioCodec> list_AudioCodecs = vkContainer->get_AudioCodecs( vkContainer->get_Containers().at(i)->get_Suffix()  );
        if ( !list_AudioCodecs.empty() )
        {
            for ( int i = 0; i < list_AudioCodecs.count(); i++ )
            {
                QIcon icon( QString::fromUtf8( "/home/vk/Programmieren/QList-Codec/codec/accept.png" ) );
                if ( list_AudioCodecs.at(i).available == false )
                {
                    icon.addFile( QString::fromUtf8( "/home/vk/Programmieren/QList-Codec/codec/missing.png" ) );
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
