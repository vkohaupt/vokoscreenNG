#ifndef COMPOSITE_H
#define COMPOSITE_H

#include <QDebug>
#include <QWidget>
#include <QPainter>
#include <QPaintEvent>
#include <QMainWindow>

class QvkComposite: public QWidget
{
    Q_OBJECT
public:
    QvkComposite( QMainWindow *parent );
    bool isCompositeEnabled = true;


public slots:


private:


private slots:
    void slot_shot();


protected:
    void paintEvent( QPaintEvent *event );

    
signals:
    void signal_compositeEnabled( bool );

};

#endif // COMPOSITE_H
