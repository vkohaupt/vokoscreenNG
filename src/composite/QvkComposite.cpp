#include "QvkComposite.h"
#include "global.h"

#include <QMainWindow>
#include <QTimer>
#include <QPaintEvent>
#include <QPixmap>
#include <QPainter>
#include <QImage>
#include <QColor>
#include <QScreen>
#include <QDebug>

QvkComposite::QvkComposite(QMainWindow *parent)
{
    Q_UNUSED(parent);
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::ToolTip);
    setAttribute(Qt::WA_TranslucentBackground, true);

    resize(5, 5);
    move(0, 0);
    show();

    QTimer::singleShot(1000,
                       Qt::PreciseTimer,
                       this,
                       [this](){
        slot_shot();
    });
}


void QvkComposite::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    QPixmap pixmap(width(), height());
    pixmap.fill(Qt::transparent);

    QPainter painter;
    painter.begin(this);
    painter.drawPixmap(0, 0, pixmap);
    painter.end();
}


void QvkComposite::slot_shot()
{
    qDebug().noquote() << global::nameOutput << "[Composite] waiting for an answer";

    QImage image = screen()->grabWindow(0, 0, 0, width(), height()).toImage();

    QColor colorBlack = image.pixelColor(2, 2);
    if (colorBlack == Qt::black){
        compositeEnabled = false;
    }else{
        compositeEnabled = true;
    }

    emit signal_compositeEnabled(compositeEnabled);
    qDebug().noquote() << global::nameOutput << "[Composite]" << compositeEnabled;
    close();
}


bool QvkComposite::get_cmpositeEnabled()
{
    return compositeEnabled;
}
