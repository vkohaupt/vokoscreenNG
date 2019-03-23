#ifndef QVKCAMERAWINDOW_H
#define QVKCAMERAWINDOW_H

#include <QLabel>
#include <QWidget>

class QvkCameraWindow : public QLabel
{
    Q_OBJECT

public:
    QvkCameraWindow();
    virtual ~QvkCameraWindow();


private:


public slots:


private slots:


protected:
    void closeEvent(QCloseEvent *event);
    void mouseDoubleClickEvent( QMouseEvent *event );
    void keyPressEvent( QKeyEvent *event );


signals:
    void signal_cameraWindow_close( bool );

};

#endif // QVKWEBCAMWINDOW_H
