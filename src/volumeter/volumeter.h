// Copyright (C) 2021 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#ifndef AUDIOINPUT_H
#define AUDIOINPUT_H

#include <QAudioSource>
#include <QMediaDevices>
#include <QByteArray>
#include <QScopedPointer>

class AudioInfo : public QIODevice
{
    Q_OBJECT

public:
    AudioInfo(const QAudioFormat &format);

    void start();
    void stop();

    qreal level() const { return m_level; }

    qint64 readData(char *data, qint64 maxlen) override;
    qint64 writeData(const char *data, qint64 len) override;

    qreal calculateLevel(const char *data, qint64 len) const;

signals:
    void levelChanged(qreal level);

private:
    const QAudioFormat m_format;
    qreal m_level = 0.0; // 0.0 <= m_level <= 1.0
};


class InputTest : public QWidget
{
    Q_OBJECT

public:
    InputTest();

private:

private slots:

private:
    QScopedPointer<AudioInfo> m_audioInfo;
    QScopedPointer<QAudioSource> m_audioInput;

    QAudioDevice myDevice;

};

#endif // AUDIOINPUT_H
