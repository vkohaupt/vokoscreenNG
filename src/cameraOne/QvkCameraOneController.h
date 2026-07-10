#ifndef QVKCAMERAONECONTROLLER_H
#define QVKCAMERAONECONTROLLER_H

#include <QWidget>
#include <QCheckBox>
#include <QButtonGroup>

#include "QvkCameraOneSingle.h"

class QvkCameraOneController : public QWidget
{
    Q_OBJECT

public:
    explicit QvkCameraOneController(QWidget *parent = nullptr);
    ~QvkCameraOneController();
    QvkCameraOneSingle *vkCameraOneSingle;
    QButtonGroup *buttonGroup = new QButtonGroup;

private:


};

#endif // QVKCAMERAONECONTROLLER_H
