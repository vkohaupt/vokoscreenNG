
#ifndef QVKSPEZIALCHECKBOX_H
#define QVKSPEZIALCHECKBOX_H

#include <QWidget>
#include <QPainter>

class QvkSpezialCheckbox : public QWidget
{
    Q_OBJECT

public:
    QvkSpezialCheckbox(QWidget *parent = 0);
    bool isChecked();

    
public slots:
    void slot_setChecked( bool value );


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


signals:
    void signal_clicked( bool );
    
};
#endif
