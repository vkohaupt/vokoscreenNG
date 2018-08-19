#ifndef settingsWidget_H
#define settingsWidget_H

#include <QWidget>
#include <QLabel>
#include <QPainter>
#include <QDebug>

class QvkWidgetSettings: public QWidget
{
    Q_OBJECT
public:    
    virtual ~QvkWidgetSettings();
    QvkWidgetSettings( QWidget *parent );

    
public:


public slots:


private:
    bool iconMode;


private slots:


protected:
    void paintEvent( QPaintEvent *event );
    void mouseReleaseEvent ( QMouseEvent * event );
    void enterEvent( QEvent *event );
    void leaveEvent( QEvent *event );


signals:
    void clicked( bool value );


};

#endif // settingsWidget_H
