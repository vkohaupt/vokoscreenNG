#ifndef QvkStorageUI_H
#define QvkStorageUI_H

#include "ui_storage.h"
#include "ui_mainwindow.h"

#include <QWidget>

class QvkStorageUI: public QWidget
{ 
Q_OBJECT
public:
  QvkStorageUI(Ui_MainWindow *ui_mainwindow);
  virtual ~QvkStorageUI();


public slots:

  
private slots: 
  void slot_systemInfo();
  void slot_newVideoFilename( QString filename );

   
signals:
  

protected:  
  
  
private:
  Ui_MainWindow *ui;
  Ui_Storage storageGUI;
  QString newVideoFilename = "";

};

#endif
