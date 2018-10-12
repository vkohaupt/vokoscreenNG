#ifndef QvkMagnifier_H 
#define QvkMagnifier_H

#include "ui_QvkMagnifierDialog.h"
#include "QvkSettings.h"

#include <QScreen>
#include <QPixmap>
#include <QDialog>
#include <QLabel>
#include <QDesktopWidget>
#include <QDebug>
#include <QTimer>
#include <QRadioButton>

class QvkMagnifier: public QDialog
{ 
Q_OBJECT
public:
  QvkMagnifier();
  virtual ~QvkMagnifier();
  

public slots:
  void slot_showDialogMagnifier();
  void slot_magnifierShow( bool value );
  

private slots:
  void slot_mytimer();
  void slot_magnifier200x200();
  void slot_magnifier400x200();
  void slot_magnifier600x200();
  

protected:  

  
signals:
  
  
private:
  int NewDistanceXLeft();
  int NewDistanceXRight();
  void setMagnifier();
  QLabel *label;
  int border;
  int distanceX;
  int distanceY;
  QRadioButton *radioButton1;
  QRadioButton *radioButton2;
  QRadioButton *radioButton3;
  int faktor;
  int formValue;
  QDialog *newDialog;
  QTimer *timer;
  QvkSettings vkSettings;
  
};


#endif
