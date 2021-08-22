#ifndef SYSTRAYALTERNATIVEWINDOW_H
#define SYSTRAYALTERNATIVEWINDOW_H

#include <QWidget>
#include <QPainter>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QContextMenuEvent>
#include <QMenu>

class QvkSystrayAlternativeWindow : public QWidget
{
    Q_OBJECT

public:
    explicit QvkSystrayAlternativeWindow( QWidget *parent );
    ~QvkSystrayAlternativeWindow();

    void set_Size( QSize m_size, QPixmap m_picture );
    void set_Menu( QMenu *m_menu );


private:
    QSize size;
    QPixmap picture;
    QPoint point;
    QMenu *menu;

private slots:


protected:
    void paintEvent(QPaintEvent *event);
    void mouseMoveEvent( QMouseEvent *event );
    void mousePressEvent( QMouseEvent *event );
    void contextMenuEvent( QContextMenuEvent *event );


};

#endif // SYSTRAYALTERNATIVEWINDOW_H
