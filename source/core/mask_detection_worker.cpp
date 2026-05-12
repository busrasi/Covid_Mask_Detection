#include "mask_detection_worker.h"

#include <QDebug>
#include <QMutexLocker>

MaskDetectionWorker::MaskDetectionWorker(MaskDetection *maskDetection, QObject *parent)
    : QObject(parent)
    , m_maskDetection(maskDetection)
{
}

void MaskDetectionWorker::detect(cv::Mat frame)
{
    qDebug() << "MaskDetectionWorker :: detect() called.";

    if (!m_maskDetection) {
        qWarning() << "MaskDetectionWorker :: MaskDetection engine is null.";
        emit detectionFinished(false);
        return;
    }

    if (frame.empty()) {
        qWarning() << "MaskDetectionWorker :: Empty frame.";
        emit detectionFinished(false);
        return;
    }

    if (!m_mutex.tryLock()) {
        qDebug() << "MaskDetectionWorker :: Detection already running, skipping.";
        return;
    }

    bool result = false;

    try {
        cv::Mat localFrame = frame.clone();
        result = m_maskDetection->executeObjDetector(localFrame);
    } catch (const std::exception &e) {
        qWarning() << "MaskDetectionWorker :: Detection exception:" << e.what();
        result = false;
    }

    m_mutex.unlock();

    qDebug() << "MaskDetectionWorker :: Detection result:" << result;

    emit detectionFinished(result);
}