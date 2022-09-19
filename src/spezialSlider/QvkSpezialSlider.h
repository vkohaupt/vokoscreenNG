#ifndef QVKSPEZIALSLIDER_H
#define QVKSPEZIALSLIDER_H

#include <QSlider>
#include <QResizeEvent>
#include <QLabel>

class QvkSpezialSlider : public QSlider
{
    Q_OBJECT

public:
    explicit QvkSpezialSlider( Qt::Orientation orientation );
    ~QvkSpezialSlider();
    QLabel *label;

    /**
    * Display a value in the handle. The default is true.
    */
    void setShowValue( bool );

    /**
    * Display a big handle. The default is true.
    */
    void setBigHandel( bool );

    /**
    * Display a value as decimal. The default is false.
    */
    void setDecimalPoint( bool );


private:
    qreal distance;
    qreal handleRadius;
    bool bigHandel = true;
    bool decimalPoint = false;
    bool showValue = true;
    int sliderHight;
    int fontWidth;
    int grooveHight;
    int grooveBorderRadius;
    QString grooveBackgroundColor;
    QString grooveForgroundColor;
    QString handleBorderColor;
    int handleBorderWith;
    void css_slider();


public slots:
    void slot_valueChanged( int );


protected:
    void resizeEvent(QResizeEvent *event);


signals:


};

#endif // QVKSPEZIALSLIDER_H
