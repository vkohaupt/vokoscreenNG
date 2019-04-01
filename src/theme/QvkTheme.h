#ifndef QVKTHEME_H 
#define QVKTHEME_H

#include "ui_formMainWindow.h"

class QvkTheme: public QObject
{ 
    Q_OBJECT
public:
    QvkTheme(Ui_formMainWindow *ui_formMainWindow );
    virtual ~QvkTheme();
    QIcon VK_getIcon( QString iconName, QString iconNameFallback );
    void makeAndSetValidIcon( QTabWidget *tabWidget, int index , QIcon icon );
    void vk_setCornerWidget( QTabWidget *tabWidget );


public slots:


private slots: 
    void slot_searchNewTheme();
    void slot_setNewTheme();


signals:
    void signal_newTheme();


protected:  


private:
    QString oldThemeName = "";
    Ui_formMainWindow *ui;

    QTimer *timer;

    void makeAndSetValidIconForSideBar( int index, QIcon icon );


};

#endif
