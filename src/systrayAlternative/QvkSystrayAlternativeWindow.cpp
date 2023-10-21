#include "QvkSystrayAlternativeWindow.h"

#include <QBitmap>
#include <QDebug>

QvkSystrayAlternativeWindow::QvkSystrayAlternativeWindow( QWidget *parent )
{
    Q_UNUSED(parent)
    setAttribute( Qt::WA_TranslucentBackground, true );
}


QvkSystrayAlternativeWindow::~QvkSystrayAlternativeWindow()
{
}


void QvkSystrayAlternativeWindow::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    QPixmap pixmap( size.width() * devicePixelRatioF(), size.height() * devicePixelRatioF() );
    pixmap.fill( Qt::transparent );
    pixmap.setDevicePixelRatio( devicePixelRatioF() );

    QPainter painterPixmap;
    painterPixmap.begin( &pixmap );
    painterPixmap.setRenderHints( QPainter::Antialiasing, true );
    painterPixmap.drawPixmap( 0, 0, picture );
    painterPixmap.end();

    QPainter painter;
    painter.begin( this );
    painter.drawPixmap( QPointF( 0, 0 ), pixmap );
    painter.end();

    setMask( pixmap.mask() );
}


void QvkSystrayAlternativeWindow::set_Size( QSize m_size, QPixmap m_picture )
{
    resize(m_size);
    size = m_size;
    picture = m_picture;
    repaint();
}


void QvkSystrayAlternativeWindow::set_Menu( QMenu *m_menu )
{
    menu = m_menu;
}


void QvkSystrayAlternativeWindow::mouseMoveEvent( QMouseEvent *event )
{
    move( event->globalPosition().toPoint() - point );
}


void QvkSystrayAlternativeWindow::mousePressEvent( QMouseEvent *event )
{
    point = event->pos();
}


void QvkSystrayAlternativeWindow::contextMenuEvent( QContextMenuEvent *event )
{
    menu->exec( event->globalPos() );
}
