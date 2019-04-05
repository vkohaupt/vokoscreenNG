#ifndef QVKSCREENMANAGER_H 
#define QVKSCREENMANAGER_H

#include <QObject>

class QvkScreenManager : public QObject
{
    Q_OBJECT

public:
    QvkScreenManager();
    virtual ~QvkScreenManager();


public slots:


private slots:
    void slot_screen_count_changed();


signals:
    void signal_clear_widget();
    void signal_screen_count_changed( QString stringText, QString stringData );


protected:  


private:

};

#endif
