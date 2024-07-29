#ifndef MASK_DETECTION_FILTER_H
#define MASK_DETECTION_FILTER_H

#include <QAbstractVideoFilter>
#include <QObject>
#include <QTemporaryFile>
#include <QMutex>

#include "source/core/mask_detection.h"

class MaskDetectionFilter : public QAbstractVideoFilter
{
        Q_OBJECT
    public:
        /**
         * @brief C-tor
         */
        MaskDetectionFilter();

        /**
         * @brief C-tor with engine param
         * @param engine
         */
        MaskDetectionFilter(MaskDetection* maskDetection);

        /**
         * @brief abstract runable override
         * @return
         */
        QVideoFilterRunnable *createFilterRunnable();

        /**
         * @brief get Mask
         * @return
         */
        MaskDetection *getMaskDetection();

    signals:
        void objectDetected(bool result);

    public slots:

    private:
        MaskDetection* m_maskDetection;

};

class MaskDetectionFilterRunnable : public QVideoFilterRunnable
{
    public:
        MaskDetectionFilterRunnable(MaskDetectionFilter *filter);
        QVideoFrame run(QVideoFrame *input, const QVideoSurfaceFormat &surfaceFormat, RunFlags flags);

    private:
        MaskDetectionFilter *filter;
        unsigned long long int frameCounter = 0;
};

#endif // MASK_DETECTION_FILTER_H
