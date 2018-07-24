#ifndef QvkWinInfo_H 
#define QvkWinInfo_H

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
  QString WinID();
  WId getWinID();
  void slot_start();

  
private slots: 
   void selectWindow();
   void mousePosition();
   WId activeWindow();

   
signals:
  void windowChanged( bool value );
  void signal_showCursor( bool value );
  
protected:  
  
  
private:
  QTimer *windowTimer;
  QTimer *mouseTimer;
  
  WId lastWinID;
  WId newWinID;
  
  void paintEvent(QPaintEvent *event);
  
};

#endif
