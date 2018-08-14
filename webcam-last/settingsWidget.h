#ifndef settingsWidget_H
#define settingsWidget_H

#include <QWidget>
#include <QLabel>
#include <QPainter>
#include <QDebug>

class settingsWidget: public QWidget
{
    Q_OBJECT
public:    
    virtual ~settingsWidget();
    settingsWidget( QWidget *parent );

    
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
