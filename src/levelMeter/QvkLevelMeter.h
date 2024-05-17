// Copyright (C) 2021 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#ifndef AUDIOINPUT_H
#define AUDIOINPUT_H

#include <QWidget>
#include <QAudioSource>
#include <QMediaDevices>
#include <QByteArray>
//#include <QScopedPointer>

class QvkQIODevice : public QIODevice
{
    Q_OBJECT

public:
    QvkQIODevice( const QAudioFormat &format );

    void start();
    void stop();

    qint64 readData(char *data, qint64 maxlen) override;
    qint64 writeData(const char *data, qint64 len) override;

    qreal calculateLevel(const char *data, qint64 len) const;

    QString myDevice;

signals:
    void signal_levelChanged(qreal level);

private:
    const QAudioFormat m_format;
};


class InputStart : public QObject
{
    Q_OBJECT

public:
    InputStart(QAudioDevice device );


public slots:
    void slot_levelChanged(qreal level);
    void slot_stop();
    void slot_start();


private:
    QvkQIODevice *vkQIODevice;
    QAudioSource *audioSource;

private slots:


signals:
    void signal_level( int );

};

#endif // AUDIOINPUT_H
