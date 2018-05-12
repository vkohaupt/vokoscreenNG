#ifndef QvkWinInfo_H 
#define QvkWinInfo_H

#include "qxtwindowsystem.h"
#include <QDialog>
#include <QTimer>
#include <QPainter>
#include <QDebug>
#include <QMainWindow>
class QvkWinInfo: public QMainWindow
{ 
Q_OBJECT
public:
  QvkWinInfo();
  virtual ~QvkWinInfo();
  
public slots:
  QString x();
  QString y();
  QString width();
  QString height();
  QString WinID();
  WId getWinID();

  
private slots: 
   void selectWindow();
   void mousePosition();

   
signals:
  void windowChanged();
  
  
protected:  
  
  
private:
  QTimer *windowTimer;
  QTimer *mouseTimer;
  
  WId lastWinID;
  WId newWinID;
  
  int myX;
  int myY;
  int myWidth;
  int myHeight;
  void paintEvent(QPaintEvent *event);
  
};

#endif
