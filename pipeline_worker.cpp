#include "pipeline_worker.h"
#include <QDebug>
#include <QTimer>

PipelineWorker::PipelineWorker(GstElement* pipeline, QObject* parent)
    : QObject(parent), m_pipeline(pipeline)
{
    // Erstelle einen Qt-Timer für das 100ms Intervall
    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &PipelineWorker::onTick100ms);

    // Starte den Timer mit 100 Millisekunden
    m_timer->start(100);
}

PipelineWorker::~PipelineWorker() {
    m_timer->stop();
}

void PipelineWorker::onTick100ms() {
    if (!m_pipeline) return;

    gint64 current_position = 0;
    gint64 total_duration = 0;

    // Position im GST_FORMAT_TIME (Nanosekunden) abfragen
    if (gst_element_query_position(m_pipeline, GST_FORMAT_TIME, &current_position) &&
        gst_element_query_duration(m_pipeline, GST_FORMAT_TIME, &total_duration))
    {
        // Umrechnung von Nanosekunden in Millisekunden für Qt
        qint64 pos_ms = current_position / 1000000;
        qint64 dur_ms = total_duration / 1000000;

        qDebug() << "Fortschritt [100ms Takt]:" << pos_ms << "ms /" << dur_ms << "ms";

        // Hier können Sie ein Qt-Signal emiten, um z.B. eine QProgressBar zu aktualisieren:
        emit progressChanged(pos_ms, dur_ms);
    }
}
