#include "QvkMagnifierController.h"

QvkMagnifierController::QvkMagnifierController(Ui_formMainWindow *ui_mainwindow ) : vkMagnifier(new QvkMagnifier)
{
    ui_vokoscreen = ui_mainwindow;
    connect( ui_vokoscreen->checkBoxMagnifier, SIGNAL( clicked( bool ) ), vkMagnifier, SLOT( slot_magnifierShow( bool ) ) );

    QDialog *newDialog = new QDialog;
    newDialog->setModal( true );
    Ui_MagnifierDialog ui_Dialog;
    ui_Dialog.setupUi( newDialog );
    connect( ui_vokoscreen->toolButtonDialogMagnifier, SIGNAL( clicked() ),       newDialog,   SLOT( show() ) );
    connect( ui_Dialog.buttonBox,                      SIGNAL( accepted() ),      newDialog,   SLOT( close() ) );
    connect( ui_Dialog.radioButton1,                   SIGNAL( clicked (bool ) ), vkMagnifier, SLOT( slot_magnifier200x200() ) );
    connect( ui_Dialog.radioButton2,                   SIGNAL( clicked (bool ) ), vkMagnifier, SLOT( slot_magnifier400x200() ) );
    connect( ui_Dialog.radioButton3,                   SIGNAL( clicked (bool ) ), vkMagnifier, SLOT( slot_magnifier600x200() ) );
}


QvkMagnifierController::~QvkMagnifierController()
{
}
