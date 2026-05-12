#ifndef MASK_DETECTION_WORKER_H
#define MASK_DETECTION_WORKER_H

#include <QObject>
#include <QMutex>

#include <opencv2/opencv.hpp>

#include "source/core/mask_detection.h"

Q_DECLARE_METATYPE(cv::Mat)

class MaskDetectionWorker : public QObject
{
    Q_OBJECT

public:
    explicit MaskDetectionWorker(MaskDetection *maskDetection, QObject *parent = nullptr);

public slots:
    void detect(cv::Mat frame);

signals:
    void detectionFinished(bool result);

private:
    MaskDetection *m_maskDetection = nullptr;
    QMutex m_mutex;
};

#endif // MASK_DETECTION_WORKER_H