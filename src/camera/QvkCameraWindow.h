#ifndef QVKCAMERAWINDOW_H
#define QVKCAMERAWINDOW_H

#include "ui_formMainWindow.h"
#include "ui_camerasettingsdialog.h"

#include "camerasettingsdialog.h"

#include <QLabel>
#include <QWidget>

class QvkCameraWindow : public QLabel
{
    Q_OBJECT

public:
    QvkCameraWindow(Ui_formMainWindow *ui_surface);
    virtual ~QvkCameraWindow();
    Ui_cameraSettingsDialog *uiCameraSettingsDialog;
    cameraSettingsDialog *vkCameraSettingsDialog;


private:
   Ui_formMainWindow *ui_formMainWindow;


public slots:


private slots:


protected:
    void closeEvent(QCloseEvent *event);
    void mouseDoubleClickEvent( QMouseEvent *event );
    void keyPressEvent( QKeyEvent *event );

    void mousePressEvent(QMouseEvent *event);


signals:
    void signal_cameraWindow_close( bool );

};

#endif // QVKWEBCAMWINDOW_H
