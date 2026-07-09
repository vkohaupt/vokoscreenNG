#ifndef QVKCAMERAONECONTROLLER_H
#define QVKCAMERAONECONTROLLER_H

#include <QWidget>
#include <QCheckBox>

#include "QvkCameraOneSingle.h"

class QvkCameraOneController : public QWidget
{
    Q_OBJECT

public:
    explicit QvkCameraOneController(QWidget *parent = nullptr);
    ~QvkCameraOneController();
    QvkCameraOneSingle *vkCameraOneSingle;

private:


};

#endif // QVKCAMERAONECONTROLLER_H
