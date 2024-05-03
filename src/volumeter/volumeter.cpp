// Copyright (C) 2021 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#include "volumeter.h"

#include <QAudioDevice>
#include <QAudioSource>
#include <QDebug>

AudioInfo::AudioInfo(const QAudioFormat &format) : m_format(format)
{
}

void AudioInfo::start()
{
    open(QIODevice::WriteOnly);
}

void AudioInfo::stop()
{
    close();
}

qint64 AudioInfo::readData( char *, qint64 )
{
    return 0;
}

qreal AudioInfo::calculateLevel( const char *data, qint64 len ) const
{
    const int channelBytes = m_format.bytesPerSample();
    const int sampleBytes = m_format.bytesPerFrame();
    const int numSamples = len / sampleBytes;

    float maxValue = 0;
    auto *ptr = reinterpret_cast<const unsigned char *>(data);

    for (int i = 0; i < numSamples; ++i) {
        for (int j = 0; j < m_format.channelCount(); ++j) {
            float value = m_format.normalizedSampleValue(ptr);

            maxValue = qMax(value, maxValue);
            ptr += channelBytes;
        }
    }
    return maxValue;
}

qint64 AudioInfo::writeData(const char *data, qint64 len)
{
    m_level = calculateLevel(data, len);
    emit levelChanged(m_level);
    qDebug() << m_level;
    return len;
}

InputTest::InputTest()
{
    QList<QAudioDevice> devices = QMediaDevices::audioInputs();
    for ( int i = 0; i < devices.count(); i++ ) {
        qDebug() << "Device: " << devices.at(i).description() << devices.at(i).id();
    }
    myDevice = devices.at(1);

    QAudioFormat format;
    format.setSampleRate(8000);
    format.setChannelCount(1);
    format.setSampleFormat(QAudioFormat::Int16);

    m_audioInfo.reset( new AudioInfo(format));
    m_audioInput.reset( new QAudioSource( myDevice, format ) );
    m_audioInfo->start();
    m_audioInput->start( m_audioInfo.data() );
}
