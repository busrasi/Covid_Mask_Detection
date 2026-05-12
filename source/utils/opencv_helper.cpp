#include "opencv_helper.h"

#include <QDebug>
#include <QVector>
#include <QRgb>

cv::Mat imageToMat8(const QImage &image)
{
    QImage img = image.convertToFormat(QImage::Format_RGB32).rgbSwapped();
    cv::Mat tmp(img.height(), img.width(), CV_8UC4,
                static_cast<void *>(img.bits()),
                img.bytesPerLine());

    return tmp.clone();
}

void ensureC3(cv::Mat *mat)
{
    Q_ASSERT(mat);
    Q_ASSERT(mat->type() == CV_8UC3 || mat->type() == CV_8UC4);

    if (mat->type() == CV_8UC4) {
        cv::Mat tmp;
        cv::cvtColor(*mat, tmp, cv::COLOR_BGRA2BGR);
        *mat = tmp;
    }
}

QImage mat8ToImage(const cv::Mat &mat)
{
    switch (mat.type()) {
    case CV_8UC1:
    {
        QVector<QRgb> colorTable;
        colorTable.reserve(256);

        for (int i = 0; i < 256; ++i)
            colorTable.append(qRgb(i, i, i));

        QImage result(mat.data,
                      mat.cols,
                      mat.rows,
                      static_cast<int>(mat.step),
                      QImage::Format_Indexed8);

        result.setColorTable(colorTable);
        return result.copy();
    }

    case CV_8UC3:
    {
        cv::Mat rgb;
        cv::cvtColor(mat, rgb, cv::COLOR_BGR2RGB);

        QImage result(rgb.data,
                      rgb.cols,
                      rgb.rows,
                      static_cast<int>(rgb.step),
                      QImage::Format_RGB888);

        return result.copy();
    }

    case CV_8UC4:
    {
        QImage result(mat.data,
                      mat.cols,
                      mat.rows,
                      static_cast<int>(mat.step),
                      QImage::Format_ARGB32);

        return result.copy();
    }

    default:
        qWarning() << "Unhandled Mat format:" << mat.type();
        return QImage();
    }
}

cv::Mat yuvFrameToMat8(QVideoFrame frame)
{
    if (!frame.isValid()) {
        qWarning() << "Invalid QVideoFrame";
        return {};
    }

    if (!frame.map(QVideoFrame::ReadOnly)) {
        qWarning() << "Could not map QVideoFrame";
        return {};
    }

    const auto pixelFormat = frame.pixelFormat();

    cv::Mat result;

    const int width = frame.width();
    const int height = frame.height();

    if (pixelFormat == QVideoFrameFormat::Format_NV12) {
        cv::Mat yuv(height + height / 2,
                    width,
                    CV_8UC1,
                    frame.bits(0));

        cv::cvtColor(yuv, result, cv::COLOR_YUV2BGR_NV12);
    }
    else if (pixelFormat == QVideoFrameFormat::Format_NV21) {
        cv::Mat yuv(height + height / 2,
                    width,
                    CV_8UC1,
                    frame.bits(0));

        cv::cvtColor(yuv, result, cv::COLOR_YUV2BGR_NV21);
    }
    else if (pixelFormat == QVideoFrameFormat::Format_YUV420P) {
        cv::Mat yuv(height + height / 2,
                    width,
                    CV_8UC1,
                    frame.bits(0));

        cv::cvtColor(yuv, result, cv::COLOR_YUV2BGR_YV12);
    }
    else {
        qWarning() << "Unsupported QVideoFrame pixel format:" << pixelFormat;
    }

    frame.unmap();

    return result;
}

void mat8ToYuvFrame(const cv::Mat &mat, uchar *dst)
{
    Q_ASSERT(mat.type() == CV_8UC3 || mat.type() == CV_8UC4);
    Q_ASSERT(dst);

    cv::Mat yuv(mat.rows + mat.rows / 2, mat.cols, CV_8UC1, dst);

    cv::cvtColor(
        mat,
        yuv,
        mat.type() == CV_8UC4
            ? cv::COLOR_BGRA2YUV_YV12
            : cv::COLOR_BGR2YUV_YV12
        );
}