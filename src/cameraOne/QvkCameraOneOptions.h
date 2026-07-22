#ifndef QVKCAMERAONEOPTIONS_H
#define QVKCAMERAONEOPTIONS_H

#include <QWidget>

#include "QvkSpezialSlider.h"

#include "ui_QvkCameraOneOptions.h"

namespace Ui {
class QvkCameraOneOptions;
}

class QvkCameraOneOptions : public QWidget
{
    Q_OBJECT

public:
    explicit QvkCameraOneOptions(QWidget *parent = nullptr);
    ~QvkCameraOneOptions();
    Ui::QvkCameraOneOptions *ui;
    void set_init(QString device);
    QvkSpezialSlider *sliderCameraOneWindowSize;
    QvkSpezialSlider *sliderCameraOneWindowZoom;
    QvkSpezialSlider *sliderCameraOneColorTemperature;


private:


};

#endif // QVKCAMERAONEOPTIONS_H
