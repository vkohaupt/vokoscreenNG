#include "QvkMagnifier_wl.h"
#include "QvkMagnifierPipeWire_wl.h"

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

    setAttribute(Qt::WA_TranslucentBackground, true);
    Qt::WindowFlags flags;
    flags  = Qt::Window;
    flags |= Qt::FramelessWindowHint;
    setWindowFlags(flags);

    showMaximized();
    setMouseTracking(true);

    vkMagnifierPipeWire_wl = new QvkMagnifierPipeWire_wl(this);
    connect(vkMagnifierPipeWire_wl, &QvkMagnifierPipeWire_wl::frameReceived, this, [this](const QImage &frame) {
        currentPicture = frame.scaled(QSize(160, 120));
        update();
    });

    // Option B: Irgendwo im Code einfach den letzten Zustand aus der Variable holen
    QImage aktuellesBild = vkMagnifierPipeWire_wl->getLatestFrame();
}

QvkMagnifier_wl::~QvkMagnifier_wl()
{
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

        int circle = 100;

        QPen pen;
        pen.setColor(QColor(0, 0, 255, 50));
        pen.setWidth(circle - 5);

        painterPixmap.setPen(pen);
        painterPixmap.setBrush(Qt::NoBrush);
        painterPixmap.drawEllipse(QPoint(m_x, m_y), circle/2, circle/2);

        painterPixmap.drawImage(m_x - currentPicture.width()/2,
                                m_y + circle,
                                currentPicture);
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
