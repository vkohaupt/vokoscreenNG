#ifndef QvkInformation_H
#define QvkInformation_H

#include "ui_mainwindow.h"

#include <QWidget>

class QvkInformation: public QWidget
{ 
Q_OBJECT
public:
  QvkInformation(Ui_MainWindow *ui_mainwindow);
  virtual ~QvkInformation();


public slots:

  
private slots: 
  void slot_systemInfo();
  void slot_newVideoFilename( QString filename );
  void slot_recordTimeStart();

  void slot_timeStart();


signals:
  

protected:  
  
  
private:
  Ui_MainWindow *ui;
  QString newVideoFilename = "";
  QTimer *timerRecord;
  QTime *summedTime;

};

#endif
