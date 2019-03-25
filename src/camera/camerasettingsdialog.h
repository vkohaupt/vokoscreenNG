#ifndef CAMERASETTINGSDIALOG_H
#define CAMERASETTINGSDIALOG_H

#include <QDialog>

namespace Ui {
class cameraSettingsDialog;
}

class cameraSettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit cameraSettingsDialog(QWidget *parent = 0);
    ~cameraSettingsDialog();
    Ui::cameraSettingsDialog *ui;

private:


};

#endif // CAMERASETTINGSDIALOG_H
