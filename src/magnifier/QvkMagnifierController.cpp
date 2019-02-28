#include "QvkMagnifierController.h"

QvkMagnifierController::QvkMagnifierController(Ui_formMainWindow *ui_mainwindow ) : vkMagnifier(new QvkMagnifier)
{
    connect( ui_mainwindow->checkBoxMagnifier,   SIGNAL( clicked( bool ) ),     vkMagnifier, SLOT( slot_magnifierShow( bool ) ) );
    connect( ui_mainwindow->sliderMagnification, SIGNAL( valueChanged( int ) ), this,        SLOT( slot_valueChanged( int ) ) );
    ui_mainwindow->sliderMagnification->setValue( 2 );
}


QvkMagnifierController::~QvkMagnifierController()
{
}


void QvkMagnifierController::slot_valueChanged( int value )
{
    if ( value == 1 )
       vkMagnifier->slot_magnifier200x200();
    if ( value == 2 )
       vkMagnifier->slot_magnifier400x200();
    if ( value == 3 )
       vkMagnifier->slot_magnifier600x200();
}
