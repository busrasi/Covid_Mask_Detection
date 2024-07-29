#include "mask_detection_filter.h"
#include "source/utils/opencv_helper.h"
#include "source/utils/rgb_frame_helper.h"

#include "opencv2/opencv.hpp"

#include <QDebug>

MaskDetectionFilter::MaskDetectionFilter()
{}

MaskDetectionFilter::MaskDetectionFilter(MaskDetection *maskDetection)
    : m_maskDetection(maskDetection)
{}


QVideoFilterRunnable* MaskDetectionFilter::createFilterRunnable()
{
    return new MaskDetectionFilterRunnable(this);
}

MaskDetection *MaskDetectionFilter::getMaskDetection()
{
    return m_maskDetection;
}


MaskDetectionFilterRunnable::MaskDetectionFilterRunnable(MaskDetectionFilter *creator)
{
    filter = creator;
}

QVideoFrame MaskDetectionFilterRunnable::run(QVideoFrame *input, const QVideoSurfaceFormat &surfaceFormat, RunFlags flags)
{
    Q_UNUSED(flags)
    Q_UNUSED(surfaceFormat)

    // Supports YUV (I420 and NV12) and RGB. The GL path is readback-based and slow.
    if (!input->isValid()
        || (input->handleType() != QAbstractVideoBuffer::NoHandle && input->handleType() != QAbstractVideoBuffer::GLTextureHandle))
    {
        qWarning("Invalid input format");
        return QVideoFrame();
    }

    cv::Mat m_mat;
    input->map(QAbstractVideoBuffer::ReadOnly);
    if (input->pixelFormat() == QVideoFrame::Format_YUV420P || input->pixelFormat() == QVideoFrame::Format_NV12)
    {
        m_mat = yuvFrameToMat8(*input);
    }
    else
    {
        QImage wrapper = RGBHelper::imageWrapper(*input);
        if (wrapper.isNull())
        {
            if (input->handleType() == QAbstractVideoBuffer::NoHandle)
                input->unmap();
            return *input;
        }

        m_mat = imageToMat8(wrapper);
    }

    ensureC3(&m_mat);

    // Mask Detection
    bool maskResult = filter->getMaskDetection()->executeObjDetector(m_mat);
    emit filter->objectDetected(maskResult);

    QImage img = mat8ToImage(m_mat);
    QVideoFrame frame(img);

    if (input->handleType() == QAbstractVideoBuffer::NoHandle) {
        input->unmap();
    }


    return frame;
}
