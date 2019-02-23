#ifndef QvkMagnifier_H 
#define QvkMagnifier_H

#include <QScreen>
#include <QPixmap>
#include <QDialog>
#include <QLabel>
#include <QDesktopWidget>
#include <QTimer>

class QvkMagnifier: public QDialog
{ 
Q_OBJECT
public:
  QvkMagnifier();
  virtual ~QvkMagnifier();
  

public slots:
  void slot_magnifierShow( bool value );
  void slot_magnifier200x200();
  void slot_magnifier400x200();
  void slot_magnifier600x200();


private slots:
  void slot_mytimer();
  

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
  int faktor;
  QTimer *timer;
  
};


#endif
