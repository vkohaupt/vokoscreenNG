#include <QObject>
#include <QTimer>
#include <gst/gst.h>

class PipelineWorker : public QObject {
    Q_OBJECT
public:
    PipelineWorker(GstElement* pipeline, QObject* parent = nullptr);
    ~PipelineWorker();

private slots:
    void onTick100ms();

private:
    GstElement* m_pipeline;
    QTimer* m_timer;
};
