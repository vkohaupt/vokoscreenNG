#ifndef QVKMAGNIFIERPIPEWIRE_WL_H
#define QVKMAGNIFIERPIPEWIRE_WL_H

// include checked
#include <QObject>
#include <QImage>
#include <QMutex>

#include <gst/gst.h>
#include <gst/app/gstappsink.h>

class QvkMagnifierPipeWire_wl : public QObject {
    Q_OBJECT

public:
    explicit QvkMagnifierPipeWire_wl(QObject *parent = nullptr);
    ~QvkMagnifierPipeWire_wl();

    bool startStream(QString vk_fd, QString vk_path);
    //bool startStream();
    void stopStream();

    // Direktzugriff auf das aktuellste Bild (Thread-sicher)
    QImage getLatestFrame();

signals:
    // Signal wird gefeuert, sobald ein neuer Frame bereitsteht
    void frameReceived(const QImage &frame);

private:
    GstElement *pipeline = nullptr;
    GstElement *appsink = nullptr;
    
    QImage currentFrame;
    QMutex frameMutex;

    // GStreamer Callback
    static GstFlowReturn onNewSample(GstAppSink *sink, gpointer userData);
};
#endif
