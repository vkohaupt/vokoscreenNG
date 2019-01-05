#ifndef QVKPLAYER_H
#define QVKPLAYER_H

#include <QWidget>
#include <QMediaPlayer>
#include <QMouseEvent>
#include <QKeyEvent>

namespace Ui {
class QvkPlayer;
}

class QvkPlayer : public QWidget
{
    Q_OBJECT

public:
    explicit QvkPlayer(QWidget *parent = 0);
    ~QvkPlayer();
    void setMediaFile( QString string );
    QString getMediaFile();


public slots:
    void slot_play();
    void slot_pause();
    void slot_stop();
    void slot_mute();

private slots:
    void slot_setNewImage( QImage image );


private:
    Ui::QvkPlayer *ui;
    QMediaPlayer *mediaPlayer;
    QString mediaFile;

protected:
    void mouseDoubleClickEvent( QMouseEvent *event );
    void closeEvent(QCloseEvent *event);
    void keyPressEvent( QKeyEvent *event );


};

#endif // QVKPLAYER_H
