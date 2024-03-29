
#ifndef QVKSPEZIALCHECKBOX_H
#define QVKSPEZIALCHECKBOX_H

#include <QWidget>
#include <QPainter>
#include <QColor>

class QvkSpezialCheckbox : public QWidget
{
    Q_OBJECT

public:
    QvkSpezialCheckbox(QWidget *parent = 0);
    bool isChecked();
    void set_colorChecked( QColor color );
    void set_colorUnChecked ( QColor color );
    void set_imageChecked( QString value );
    void set_imageUnChecked( QString value );
    void set_waitBeforeTrigger( int value );


public slots:
    void slot_setChecked( bool value );
    void slot_click();


protected:
    void paintEvent(QPaintEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;


private:
    int step;

    qreal button_x;
    qreal button_y;
    qreal button_width;
    qreal button_height;
    qreal penWidth;
    qreal radius;
    qreal margin;
    
    bool checked = false;

    void paintChecked( QPainter &painter );
    void paintUnChecked( QPainter &painter );

    QColor colorChecked;
    QColor colorUnChecked;

    QString imageChecked;
    QString imageUnChecked;

    int waitBeforeTrigger = 0;


signals:
    void signal_clicked( bool );
    
};
#endif
