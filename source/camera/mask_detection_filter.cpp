#include "mask_detection_filter.h"

#include "source/utils/opencv_helper.h"
#include "source/utils/rgb_frame_helper.h"

#include <QDebug>
#include <QVideoFrameFormat>
#include <QDir>

MaskDetectionFilter::MaskDetectionFilter(QObject *parent)
    : QObject(parent)
{
    m_timer.start();
}

MaskDetectionFilter::MaskDetectionFilter(MaskDetection *maskDetection, QObject *parent)
    : QObject(parent)
    , m_maskDetection(maskDetection)
{
    m_timer.start();

    m_worker = new MaskDetectionWorker(m_maskDetection);
    m_worker->moveToThread(&m_workerThread);

    connect(this,
            &MaskDetectionFilter::frameReadyForDetection,
            m_worker,
            &MaskDetectionWorker::detect,
            Qt::QueuedConnection);

    connect(m_worker,
            &MaskDetectionWorker::detectionFinished,
            this,
            [this](bool result) {
                qDebug() << "MaskDetectionFilter :: Detection finished:" << result;
                m_detectionBusy = false;
                emit objectDetected(result);
            },
            Qt::QueuedConnection);

    m_workerThread.start();

    qDebug() << "MaskDetectionFilter :: Worker thread started.";
}

MaskDetectionFilter::~MaskDetectionFilter()
{
    m_workerThread.quit();
    m_workerThread.wait();

    delete m_worker;
    m_worker = nullptr;
}

void MaskDetectionFilter::processFrame(const QVideoFrame &frame)
{
    if (!frame.isValid()) {
        qWarning() << "MaskDetectionFilter :: Invalid frame.";
        return;
    }

    if (!m_maskDetection || !m_worker) {
        qWarning() << "MaskDetectionFilter :: Missing maskDetection or worker.";
        return;
    }

    if (m_detectionBusy) {
        return;
    }

    if (m_timer.elapsed() < 1000) {
        return;
    }

    m_timer.restart();

    QVideoFrame inputFrame(frame);

    if (!inputFrame.map(QVideoFrame::ReadOnly)) {
        qWarning() << "MaskDetectionFilter :: Could not map frame.";
        return;
    }

    cv::Mat mat;

    const auto format = inputFrame.pixelFormat();

    qDebug() << "MaskDetectionFilter :: Processing one frame.";
    qDebug() << "MaskDetectionFilter :: Frame size:"
             << inputFrame.size()
             << "format:" << format
             << "plane count:" << inputFrame.planeCount();

    const int width = inputFrame.width();
    const int height = inputFrame.height();

    if (format == QVideoFrameFormat::Format_NV21 ||
        format == QVideoFrameFormat::Format_NV12) {

        uchar *yPlane = inputFrame.bits(0);
        uchar *uvPlane = inputFrame.bits(1);

        int yStride = inputFrame.bytesPerLine(0);
        int uvStride = inputFrame.bytesPerLine(1);

        cv::Mat yMat(height, width, CV_8UC1, yPlane, yStride);
        cv::Mat uvMat(height / 2, width / 2, CV_8UC2, uvPlane, uvStride);

        if (format == QVideoFrameFormat::Format_NV21) {
            cv::cvtColorTwoPlane(yMat, uvMat, mat, cv::COLOR_YUV2BGR_NV21);
        } else {
            cv::cvtColorTwoPlane(yMat, uvMat, mat, cv::COLOR_YUV2BGR_NV12);
        }
    }
    else if (format == QVideoFrameFormat::Format_YUV420P) {

        uchar *yPlane = inputFrame.bits(0);
        uchar *uPlane = inputFrame.bits(1);
        uchar *vPlane = inputFrame.bits(2);

        int yStride = inputFrame.bytesPerLine(0);
        int uStride = inputFrame.bytesPerLine(1);
        int vStride = inputFrame.bytesPerLine(2);

        cv::Mat yMat(height, width, CV_8UC1, yPlane, yStride);
        cv::Mat uMat(height / 2, width / 2, CV_8UC1, uPlane, uStride);
        cv::Mat vMat(height / 2, width / 2, CV_8UC1, vPlane, vStride);

        cv::Mat uResized;
        cv::Mat vResized;

        cv::resize(uMat, uResized, cv::Size(width, height), 0, 0, cv::INTER_LINEAR);
        cv::resize(vMat, vResized, cv::Size(width, height), 0, 0, cv::INTER_LINEAR);

        std::vector<cv::Mat> yuvChannels = { yMat, uResized, vResized };

        cv::Mat yuv;
        cv::merge(yuvChannels, yuv);

        cv::cvtColor(yuv, mat, cv::COLOR_YUV2BGR);
    }
    else {
        inputFrame.unmap();

        QVideoFrame imageFrame(frame);
        QImage image = RGBHelper::imageWrapper(imageFrame);

        if (image.isNull()) {
            qWarning() << "MaskDetectionFilter :: imageWrapper returned null.";
            return;
        }

        mat = imageToMat8(image);

        if (mat.empty()) {
            qWarning() << "MaskDetectionFilter :: Empty cv::Mat after QImage conversion.";
            return;
        }

        ensureC3(&mat);
        goto afterConversion;
    }

    inputFrame.unmap();

    if (mat.empty()) {
        qWarning() << "MaskDetectionFilter :: Empty cv::Mat after conversion.";
        return;
    }

    ensureC3(&mat);

afterConversion:

    /*
        Android camera buffer çoğunlukla landscape gelir.
        Telefon dikey tutuluyorsa 1920x1080 frame aslında yan dönmüş olur.
        Eğer kaydedilen debug frame yan ise ROTATE_90_CLOCKWISE / COUNTERCLOCKWISE denenir.
    */

    if (mat.cols > mat.rows) {
        qDebug() << "MaskDetectionFilter :: Rotating landscape frame to portrait.";

        cv::Mat rotated;

        // Eğer görüntü baş aşağı olduysa bunu COUNTERCLOCKWISE yap.
        cv::rotate(mat, rotated, cv::ROTATE_90_COUNTERCLOCKWISE);

        mat = rotated;
    }

    qDebug() << "MaskDetectionFilter :: Converted Mat rows:"
             << mat.rows
             << "cols:" << mat.cols
             << "channels:" << mat.channels()
             << "type:" << mat.type();

    static int savedFrameCount = 0;

    if (savedFrameCount < 10) {
        QString dir = "/sdcard/Download/MaskDefenceDebug";

        QDir().mkpath(dir);

        QString path =
            dir + QString("/camera_frame_%1.jpg").arg(savedFrameCount);

        bool saved = cv::imwrite(path.toStdString(), mat);

        qDebug() << "MaskDetectionFilter :: Saved frame:"
                 << path
                 << "success:" << saved;

        savedFrameCount++;
    }

    m_detectionBusy = true;

    qDebug() << "MaskDetectionFilter :: Sending frame to worker.";

    emit frameReadyForDetection(mat.clone());
}