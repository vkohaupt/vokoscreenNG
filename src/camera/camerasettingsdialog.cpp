#include "ui_camerasettingsdialog.h"

#include "camerasettingsdialog.h"
#include "global.h"

cameraSettingsDialog::cameraSettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::cameraSettingsDialog)
{
    setWindowFlags( Qt::Window | Qt::WindowStaysOnTopHint );
    ui->setupUi(this);
    ui->tabWidget->setTabIcon( 0, QIcon::fromTheme( "camera-web", QIcon( ":/pictures/camera/camera.png" ) ) );
    setWindowIcon( QIcon( ":/pictures/logo.png" ) );
    setWindowTitle( global::name + " " + tr( "Camera settings" ) );
}

cameraSettingsDialog::~cameraSettingsDialog()
{
    delete ui;
}
