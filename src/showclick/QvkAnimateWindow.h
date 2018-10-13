#ifndef QvkAnimateWindow_H
#define QvkAnimateWindow_H

#include <QDialog>


class QvkAnimateWindow: public QDialog

{
Q_OBJECT
public:    
    QvkAnimateWindow();
    virtual ~QvkAnimateWindow();
    
public:


public slots:
  void setRadiusColor( int valueRadius, QColor valueColor );
  void setOpacity( double value );
  void setRadiant( bool value );

  
private:
  int diameter;
  QColor color;
  double opacity;
  bool radiant;
  
private slots:

  
protected:
  void paintEvent(QPaintEvent *event);

  
};

#endif // QvkAnimateWindow_H
