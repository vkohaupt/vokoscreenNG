#ifndef QVKSETTINGS_H 
#define QVKSETTINGS_H

#include "ui_formMainWindow.h"
#include "QvkRegionChoise.h"

#include <QObject>
#include <QSettings>
#include <QDebug>
#include <QMainWindow>

class QvkSettings: public QObject
{ 
    Q_OBJECT
public:
    QvkSettings();
    virtual ~QvkSettings();
    void saveAll(Ui_formMainWindow *ui_mainwindow, QMainWindow *parent );
    void readAll(Ui_formMainWindow *ui_mainwindow, QMainWindow *parent);
    void saveArea(int x, int y, int width, int height);
    void readArea(QvkRegionChoise *vkRegionChoise);

    QString getFileName();


public slots:


private slots:


signals:


protected:  


private:

};

#endif
