#ifndef QvkInformation_H
#define QvkInformation_H

#include "ui_formMainWindow.h"

#include <QWidget>

class QvkInformation: public QWidget
{ 
Q_OBJECT
public:
  QvkInformation( QMainWindow *mainWindow, Ui_formMainWindow *ui_mainwindow);
  virtual ~QvkInformation();


public slots:

  
private slots: 
  void slot_cleanup();
  void slot_StorageInfo();
  void slot_newVideoFilename( QString filename );
  void slot_displayRecordTime();

  void slot_timeFirstStart();
  void slot_timeContinueStart();

  void slot_summedTimeAfterPause();


signals:
  

protected:  

  
private:
  Ui_formMainWindow *ui;
  QString newVideoFilename = "";
  QTimer *timerStorageInfo;
  QTimer *timerRecord;
  QTime *elapsedTime;

  int int_summed = 0;
};

#endif
