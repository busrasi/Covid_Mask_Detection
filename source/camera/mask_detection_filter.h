#ifndef MASK_DETECTION_FILTER_H
#define MASK_DETECTION_FILTER_H

#include <QObject>
#include <QVideoFrame>
#include <QElapsedTimer>
#include <QThread>

#include <opencv2/opencv.hpp>

#include "source/core/mask_detection.h"
#include "source/core/mask_detection_worker.h"

class MaskDetectionFilter : public QObject
{
    Q_OBJECT

public:
    explicit MaskDetectionFilter(QObject *parent = nullptr);
    explicit MaskDetectionFilter(MaskDetection *maskDetection, QObject *parent = nullptr);
    ~MaskDetectionFilter();

signals:
    void objectDetected(bool result);
    void frameReadyForDetection(const cv::Mat frame);

public slots:
    void processFrame(const QVideoFrame &frame);

private:
    MaskDetection *m_maskDetection = nullptr;

    QThread m_workerThread;
    MaskDetectionWorker *m_worker = nullptr;

    QElapsedTimer m_timer;
    bool m_detectionBusy = false;
};

#endif // MASK_DETECTION_FILTER_H