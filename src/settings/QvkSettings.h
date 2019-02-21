#ifndef QVKSETTINGS_H 
#define QVKSETTINGS_H

#include "ui_formMainWindow.h"

#include <QObject>
#include <QSettings>
#include <QDebug>
#include <QColor>

class QvkSettings: public QObject
{ 
    Q_OBJECT
public:
    QvkSettings();
    virtual ~QvkSettings();
    void saveAll(Ui_formMainWindow *ui_mainwindow);
    void readAll(Ui_formMainWindow *ui_mainwindow);

    QString getVersion();
    QString getProgName();


public slots:
    // ShowClick
    int    getShowClickCircle();
    bool   getShowClickRadiant();
    QColor getShowClickColor();
    double getShowClickOpacity();
    double getShowClickTime();

    // Magnifier
    int getMagnifierFormValue();


private slots:


signals:


protected:  


private:
    QString ProgName;
    QString Version;

    // ShowClick
    int showClickCircle;
    bool showClickRadiant;
    QColor showClickColor;
    double showClickOpacity;
    double showClickTime;

    // Magnifier
    int magnifierFormValue;
};

#endif
