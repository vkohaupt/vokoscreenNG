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
  WId getWinID();


public slots:
  void slot_start();

  
private slots: 
   void slot_selectWindow();
   void slot_mousePosition();

   
signals:
  void signal_windowChanged( bool value );
  void signal_showCursor( bool value );
  
protected:  
  
  
private:
  WId activeWindow();

  QTimer *windowTimer;
  QTimer *mouseTimer;
  
  WId lastWinID;
  WId newWinID;
  
  void paintEvent(QPaintEvent *event);
  
};

#endif
