#ifndef regionButtonArrow_H
#define regionButtonArrow_H

#include <QPainter>

class QvkRegionButtonArrow : public QObject
{ 
Q_OBJECT
public:
  QvkRegionButtonArrow();
  virtual ~QvkRegionButtonArrow();
  enum degreeArrow { topMiddle=0, topRight=45, rightMiddle=90, bottomRight=135,
                     bottomMiddel=180, bottomLeft=225, leftMiddel=270, topLeft=315 };

private:
  qreal penWidth = 2.0;
  qreal penWidthHalf = penWidth/2.0;
  qreal radius = 20.0;
  qreal diameter = 2.0 * radius;

public slots:
  QPixmap getPixmapHandle( Qt::GlobalColor color, degreeArrow degree );
  QPixmap getButton( Qt::GlobalColor color );
  QPixmap getArrow( degreeArrow degree );
  int getWithHalf();


private slots:


protected:

    


};
#endif
