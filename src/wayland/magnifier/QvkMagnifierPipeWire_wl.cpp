#include "QvkMagnifierPipeWire_wl.h"
#include <QDebug>

QvkMagnifierPipeWire_wl::QvkMagnifierPipeWire_wl(QObject *parent) : QObject(parent) {
    gst_init(nullptr, nullptr);
}

QvkMagnifierPipeWire_wl::~QvkMagnifierPipeWire_wl() {
    stopStream();
}

bool QvkMagnifierPipeWire_wl::startStream() {
    // Pipeline-String für Wayland/PipeWire -> RGBA Format für QImage
    const char* pipelineStr = 
        "pipewiresrc ! "
        "videoconvert ! "
        "video/x-raw,format=RGBA ! "
        "appsink name=sink emit-signals=true max-buffers=1 drop=true";

    GError *error = nullptr;
    pipeline = gst_parse_launch(pipelineStr, &error);

    if (error) {
        qCritical() << "GStreamer Pipeline-Fehler:" << error->message;
        g_error_free(error);
        return false;
    }

    appsink = gst_bin_get_by_name(GST_BIN(pipeline), "sink");
    
    // Callback für neue Frames registrieren
    GstAppSinkCallbacks callbacks = { nullptr, nullptr, onNewSample, nullptr, nullptr, nullptr };
    gst_app_sink_set_callbacks(GST_APP_SINK(appsink), &callbacks, this, nullptr);

    // Stream starten (Unter Wayland öffnet sich hier der Portal-Auswahldialog)
    GstStateChangeReturn ret = gst_element_set_state(pipeline, GST_STATE_PLAYING);
    if (ret == GST_STATE_CHANGE_FAILURE) {
        qCritical() << "Fehler beim Starten der GStreamer-Pipeline.";
        return false;
    }

    return true;
}

void QvkMagnifierPipeWire_wl::stopStream() {
    if (pipeline) {
        gst_element_set_state(pipeline, GST_STATE_NULL);
        gst_object_unref(appsink);
        gst_object_unref(pipeline);
        pipeline = nullptr;
        appsink = nullptr;
    }
}

QImage QvkMagnifierPipeWire_wl::getLatestFrame() {
    QMutexLocker locker(&frameMutex);
    return currentFrame;
}

// Dieser Callback wird im GStreamer-Thread ausgeführt
GstFlowReturn QvkMagnifierPipeWire_wl::onNewSample(GstAppSink *sink, gpointer userData) {
    auto *streamer = static_cast<QvkMagnifierPipeWire_wl*>(userData);

    // Frame aus der Pipeline ziehen
    GstSample *sample = gst_app_sink_pull_sample(sink);
    if (!sample) return GST_FLOW_ERROR;

    GstBuffer *buffer = gst_sample_get_buffer(sample);
    GstCaps *caps = gst_sample_get_caps(sample);
    GstStructure *structure = gst_caps_get_structure(caps, 0);

    int width = 0, height = 0;
    gst_structure_get_int(structure, "width", &width);
    gst_structure_get_int(structure, "height", &height);

    GstMapInfo map;
    if (gst_buffer_map(buffer, &map, GST_MAP_READ)) {
        // Daten in ein QImage konvertieren (Deep Copy, da GStreamer-Buffer gleich freigegeben wird)
        QImage img(map.data, width, height, QImage::Format_RGBA8888);
        QImage clonedImg = img.copy(); 

        gst_buffer_unmap(buffer, &map);

        // Frame Thread-sicher in die Variable schreiben
        {
            QMutexLocker locker(&streamer->frameMutex);
            streamer->currentFrame = clonedImg;
        }

        // Signal an den Qt-Main-Thread senden
        emit streamer->frameReceived(clonedImg);
    }

    gst_sample_unref(sample);
    return GST_FLOW_OK;
}
