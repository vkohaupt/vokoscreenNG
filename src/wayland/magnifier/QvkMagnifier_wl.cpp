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
    m_x = 300;
    m_y = 300;
    circle = 100;
    m_bluePen.setColor(QColor(0, 0, 255, 1));
    m_bluePen.setWidth(circle - 5);
    m_grayPen.setColor(Qt::gray);
    m_grayPen.setWidth(2);

    setAttribute(Qt::WA_TranslucentBackground, true);
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
    emit signal_MagnifierClose();
}


void QvkMagnifier_wl::slot_setCurrentPicture(QImage image)
{
    QImage img = image.copy(m_x - 100, m_y - 50, 200, 100);
    currentPicture = img.scaled(QSize(400, 200),
                                Qt::KeepAspectRatio,
                                Qt::SmoothTransformation);

    update();
}


void QvkMagnifier_wl::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QPixmap pixmap(width(), height());
    pixmap.fill(Qt::transparent);

    QPainter painterPixmap;
    painterPixmap.begin(&pixmap);
    {
        painterPixmap.setRenderHint(QPainter::Antialiasing, true);
        painterPixmap.setRenderHint(QPainter::SmoothPixmapTransform, true);

        painterPixmap.setPen(m_bluePen);
        painterPixmap.setBrush(Qt::NoBrush);
        painterPixmap.drawEllipse(QPoint(m_x, m_y), circle/2, circle/2);

        painterPixmap.drawImage(m_x - currentPicture.width()/2,
                                m_y + 50,
                                currentPicture);

        painterPixmap.setPen(m_grayPen);
        painterPixmap.drawRect(m_x - currentPicture.width()/2,
                               m_y + 50,
                               currentPicture.width(),
                               currentPicture.height());
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

    clearMask();
    setMask(pixmap.mask());
}



void QvkMagnifier_wl::mouseMoveEvent(QMouseEvent *event)
{
    m_x = event->position().x();
    m_y = event->position().y();
    update();
}
