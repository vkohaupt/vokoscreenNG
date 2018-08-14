#include "settingsWidget.h"

#include <QIcon>

settingsWidget::settingsWidget( QWidget *parent ) : QWidget( parent )
{
    iconMode = false;
}


settingsWidget::~settingsWidget()
{
}

void settingsWidget::paintEvent( QPaintEvent *event )
{
    Q_UNUSED(event);
    QIcon icon = QIcon::fromTheme( "preferences-system", QIcon( ":/pictures/webcam.png" ) );
    QPixmap iconPixmap = icon.pixmap( QSize( width(), height() ) );

    QPainter painter;
    painter.begin( this );
    painter.setRenderHints( QPainter::Antialiasing, true );
    if ( iconMode == false )
    {
        painter.drawPixmap( 0, 0, width(), height(), iconPixmap );
    }
    else
    {
        painter.drawPixmap( 0, 0, width()+5, height()+5, iconPixmap );
    }
    painter.end();
}

/*
 * Wird ausgeführt beim Losslassen der Maustaste
 */
void settingsWidget::mouseReleaseEvent ( QMouseEvent * event )
{
    Q_UNUSED(event);
    repaint();
}


void settingsWidget::enterEvent( QEvent *event )
{
    Q_UNUSED(event);
    iconMode = true;
}


void settingsWidget::leaveEvent( QEvent *event )
{
    Q_UNUSED(event);
    iconMode = false;
}
