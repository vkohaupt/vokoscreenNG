#ifndef QvkWinInfo_H 
#define QvkWinInfo_H

#include <QTimer>
#include <QPainter>
#include <QMainWindow>

#ifdef Q_OS_LINUX
typedef QList<WId> WindowList;
#endif

class QvkWinInfo: public QMainWindow
{ 
    Q_OBJECT
public:
    QvkWinInfo();
    virtual ~QvkWinInfo();
    WId getWinID();


public slots:
    void slot_start();


private slots: 
    void slot_selectWindow();
    void slot_mousePosition();


signals:
    void signal_windowChanged( bool value );
    void signal_showCursor( bool value );

protected:  


private:
    static WId activeWindow();

    QTimer *windowTimer;
    QTimer *mouseTimer;

    WId lastWinID;
    WId newWinID;

    void paintEvent(QPaintEvent *event);

};

#endif
