#include "settingsWidget.h"

settingsWidget::settingsWidget( QWidget *parent ) : QWidget( parent )
{
}

settingsWidget::~settingsWidget()
{
}

void settingsWidget::paintEvent( QPaintEvent *event )
{
  (void)event;

  QPixmap pixmap( "/vkmnt/daten/Daten/Volker/Programmieren/qt/vokoscreenplus/myWidget/settings.png" );

  QPainter painter;
  painter.begin( this );
     painter.setRenderHints( QPainter::Antialiasing, true );
     painter.drawPixmap( 0, 0, 30, 30 , pixmap);
  painter.end();

}

/*
 * Wird ausgefühert beim Losslassen der Maustaste
 */
void settingsWidget::mouseReleaseEvent ( QMouseEvent * event )
{
  (void)event;
  repaint();
}

void settingsWidget::setColor( const QColor color)
{
  widgetColor = color;
}
