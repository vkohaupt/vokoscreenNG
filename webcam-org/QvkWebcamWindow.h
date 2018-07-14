#ifndef QvkWebcamWindow_H 
#define QvkWebcamWindow_H

#include <QMainWindow>
#include <QLabel>
#include <QAction>
#include <QMenu>
#include <QContextMenuEvent>

//#include "QvkSettings.h"

class QvkWebcamWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    QvkWebcamWindow();
    virtual ~QvkWebcamWindow();
    QString currentDevice;
    QLabel *webcamLabel;

    QAction *actionLabelWindowSize;
    QAction *action160x120;
    QAction *action320x240;
    QAction *action640x480;
    QAction *actionLabelOptions;
    QAction *actionUserDefined;
    QAction *actionBorder;
#ifndef Q_OS_WIN
    QAction *actionVisibleOverFullscreen;
#endif
    QAction *actionClose;
    QMenu menu;


private:
    //QvkSettings vkSettings;
    bool border;
    bool overFullScreen;


public slots:
    int getValueX();
    int getValueY();
    int getValueWidth();
    int getValueHeight();
    bool getValueBorder();
#ifndef Q_OS_WIN
    bool getOverFullScreen();
    void visibleOverFullscreen();
    void overFullScreenSetWindowFlags();
#endif

private slots:
    void set160x120();
    void set320x240();
    void set640x480();
    void setActionUserDefined();
    void setBorder(bool value);
    void closeMenue();
    void setValueBorder( bool value );
#ifndef Q_OS_WIN
    void setOverFullScreen( bool value );
#endif

protected:
    void closeEvent( QCloseEvent * event );
    void contextMenuEvent( QContextMenuEvent *event );
    void resizeEvent (QResizeEvent * event);
    
signals:
    void closeWebcamWindow();
    void webcamWindoResize( QSize size );
#ifndef Q_OS_WIN
    void setOverScreen();
#endif

};

#endif
