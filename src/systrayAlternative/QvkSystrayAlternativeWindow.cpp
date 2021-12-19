#include "QvkSystrayAlternativeWindow.h"

#include <QBitmap>
#include <QDebug>

#ifdef Q_OS_LINUX
#include <QX11Info>
#endif


QvkSystrayAlternativeWindow::QvkSystrayAlternativeWindow( QWidget *parent )
{
    Q_UNUSED(parent)

#ifdef Q_OS_LINUX
    if ( QX11Info::isCompositingManagerRunning() == true )
    {
        setAttribute( Qt::WA_TranslucentBackground, true );
    } else
    {
        setAttribute( Qt::WA_TranslucentBackground, false );
    }
#endif

#ifdef Q_OS_WIN
    setAttribute( Qt::WA_TranslucentBackground, true );
#endif
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
    move( event->globalPos() - point );
}


void QvkSystrayAlternativeWindow::mousePressEvent( QMouseEvent *event )
{
    point = event->pos();
}


void QvkSystrayAlternativeWindow::contextMenuEvent( QContextMenuEvent *event )
{
    menu->exec( event->globalPos() );
}
