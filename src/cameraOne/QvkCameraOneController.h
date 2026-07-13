#ifndef QVKCAMERAONECONTROLLER_H
#define QVKCAMERAONECONTROLLER_H

#include <QWidget>
#include <QButtonGroup>

#include "QvkCameraOneSingle.h"

#include "ui_formMainWindow.h"

class QvkCameraOneController : public QWidget
{
    Q_OBJECT

public:
    explicit QvkCameraOneController(QWidget *parent, Ui_formMainWindow *m_GuiUi);
    ~QvkCameraOneController();
    QvkCameraOneSingle *vkCameraOneSingle;
    QButtonGroup *buttonGroup = new QButtonGroup;

private:
    Ui_formMainWindow *GuiUi;


};

#endif // QVKCAMERAONECONTROLLER_H
