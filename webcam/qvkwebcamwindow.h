#ifndef QVKWEBCAMWINDOW_H
#define QVKWEBCAMWINDOW_H

#include "frameWidget.h"
#include "settingsWidget.h"
#include "QvkWidgetHelp.h"
#include "QvkWidgetMenueBackground.h"
#include "QvkWidgetExit.h"

#include <QLabel>

class QvkWebcamWindow : public QLabel
{
    Q_OBJECT

public:
    QvkWebcamWindow();
    ~QvkWebcamWindow();


private:
    QvkWidgetMenueBackground *widgetMenueBackground;
    QvkWidgetHelp *myWidgetHelp;
    frameWidget *myFrameWidget;
    settingsWidget *mySettingsWidget;
    QvkWidgetExit *myWidgetExit;
    QSize widgetSize;
    int margin;


public slots:


private slots:
    void slot_set160x120();
    void slot_set320x240();
    void slot_set640x480();
    void slot_closeWebcamWindow();
    void slot_frameOnOff( bool value );


protected:
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);
    void closeEvent(QCloseEvent *event);
    void resizeEvent(QResizeEvent *event);


signals:
    void signal_webcamwindow_close();

};

#endif // QVKWEBCAMWINDOW_H
