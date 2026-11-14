#include "QvkMagnifier_wl.h"
#include "QvkMagnifierPipeWire_wl.h"
#include "QvkMagnifierPortal_wl.h"

#include <QPainter>
#include <QPixmap>
#include <QPen>
#include <QPointF>
#include <QPoint>
#include <QImage>
#include <QColor>
#include <QSize>
#include <QMouseEvent>

QvkMagnifier_wl::QvkMagnifier_wl(QWidget *parent)
{
    Q_UNUSED(parent)
    mouse_x = 300;
    mouse_y = 300;
    circle = 100;
    b_c_m = 10;// b_c_m = between_circle_margin Abstand zwischen circle und Lupe
    circlePen.setColor(QColor(0, 0, 255, 10));
    circlePen.setWidth(circle - 3);
    magnifierPen.setColor(Qt::gray);
    magnifierPen.setWidth(2);
    magnifierWidth = 400;  // Breite der Lupe
    magnifierHeight = 200; // Höhe der Lupe
    sourceWidth = magnifierWidth/2;
    sourceHeight = magnifierHeight/2;

    setAttribute(Qt::WA_TranslucentBackground, true);
    setAttribute(Qt::WA_QuitOnClose, false);
    Qt::WindowFlags flags;
    flags  = Qt::Window;
    flags |= Qt::FramelessWindowHint;
    setWindowFlags(flags);
    setMouseTracking(true);
    showMaximized();
}

QvkMagnifier_wl::~QvkMagnifier_wl()
{
}


void QvkMagnifier_wl::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event)
    emit signal_MagnifierClose();
}


void QvkMagnifier_wl::slot_setCurrentPicture(QImage image)
{
    if (mouse_y < 50){
        QImage img = image.copy(mouse_x - sourceWidth/2, 0, sourceWidth, sourceHeight);
        currentPicture = img.scaled(QSize(magnifierWidth, magnifierHeight),
                                    Qt::KeepAspectRatio,
                                    Qt::SmoothTransformation);
        update();
        return;
    }

    QImage img = image.copy(mouse_x - sourceWidth/2,
                            mouse_y - sourceHeight/2,
                            sourceWidth,
                            sourceHeight);
    currentPicture = img.scaled(QSize(magnifierWidth, magnifierHeight),
                                Qt::KeepAspectRatio,
                                Qt::SmoothTransformation);
    update();
}


void QvkMagnifier_wl::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    pixmap = QPixmap(width(), height());
    pixmap.fill(Qt::transparent);

    QPainter painterPixmap;
    painterPixmap.begin(&pixmap);
    {
        painterPixmap.setRenderHint(QPainter::Antialiasing, true);
        painterPixmap.setRenderHint(QPainter::SmoothPixmapTransform, true);

        painterPixmap.setPen(circlePen);
        painterPixmap.setBrush(Qt::NoBrush);
        painterPixmap.drawEllipse(QPoint(mouse_x, mouse_y), circle/2, circle/2);

        // Top
        if (mouse_y < 50){
            painterPixmap.drawImage(mouse_x - currentPicture.width()/2,
                                    100 + b_c_m,
                                    currentPicture);
            painterPixmap.setPen(magnifierPen);
            painterPixmap.drawRect(mouse_x - currentPicture.width()/2,
                                   100 + b_c_m,
                                   currentPicture.width(),
                                   currentPicture.height());
        }

        // Middle
        if ((mouse_y <= (screen()->size().height() - 250 - b_c_m)) and (mouse_y >= 50)){
            painterPixmap.drawImage(mouse_x - currentPicture.width()/2,
                                    mouse_y + 50 + b_c_m,
                                    currentPicture);

            painterPixmap.setPen(magnifierPen);
            painterPixmap.drawRect(mouse_x - currentPicture.width()/2,
                                   mouse_y + 50 + b_c_m,
                                   currentPicture.width(),
                                   currentPicture.height());
        }

        // Bottom
        if (mouse_y > (screen()->size().height() - 250 - b_c_m)){
            painterPixmap.drawImage(mouse_x - currentPicture.width()/2,
                                    mouse_y - 250 - b_c_m,
                                    currentPicture);
            painterPixmap.setPen(magnifierPen);
            painterPixmap.drawRect(mouse_x - currentPicture.width()/2,
                                   mouse_y - 250 - b_c_m,
                                   currentPicture.width(),
                                   currentPicture.height());
        }
    }
    painterPixmap.end();

    QPainter painter;
    painter.begin(this);
    {
        painter.setRenderHint(QPainter::Antialiasing, true);
        painter.setRenderHint(QPainter::SmoothPixmapTransform, true);
        painter.drawPixmap(QPointF(0, 0), pixmap);
    }
    painter.end();

    setMask(pixmap.mask());
}



void QvkMagnifier_wl::mouseMoveEvent(QMouseEvent *event)
{
    mouse_x = event->position().x();
    mouse_y = event->position().y();
    update();
}
