#ifndef QVKCAMERAWINDOW_H
#define QVKCAMERAWINDOW_H

#include "QvkWidgetFrame.h"
#include "QvkWidgetSettings.h"
#include "QvkWidgetHelp.h"
#include "QvkWidgetMenueBackground.h"
#include "QvkWidgetExit.h"

#include <QLabel>

class QvkCameraWindow : public QLabel
{
    Q_OBJECT

public:
    QvkCameraWindow();
    virtual ~QvkCameraWindow();


private:
    QvkWidgetMenueBackground *widgetMenueBackground;
    QvkWidgetHelp *myWidgetHelp;
    frameWidget *myFrameWidget;
    QvkWidgetSettings *mySettingsWidget;
    QvkWidgetExit *myWidgetExit;
    QSize widgetSize;
    int margin;


public slots:


private slots:
    void slot_set160x120();
    void slot_set320x240();
    void slot_set640x480();
    void slot_frameOnOff( bool value );


protected:
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);
    void closeEvent(QCloseEvent *event);
    void resizeEvent(QResizeEvent *event);


signals:
    void signal_cameraWindow_close( bool );

};

#endif // QVKWEBCAMWINDOW_H
