#include "mask_detection.h"

#include <QDebug>
#include <QFile>
#include <QDir>
#include <QStandardPaths>

#include <dlib/opencv/cv_image.h>
#include <dlib/geometry.h>

namespace dlib
{
inline cv::Rect dlibRectangleToOpenCV(dlib::rectangle r)
{
    return cv::Rect(
        cv::Point2i(r.left(), r.top()),
        cv::Point2i(r.right() + 1, r.bottom() + 1)
        );
}
}

MaskDetection::MaskDetection(const QString &maskDataPath)
    : m_maskDataPath(maskDataPath)
{
    qDebug() << "MaskDetection :: Loading model from:" << m_maskDataPath;

    try {
        dlib::deserialize(m_maskDataPath.toStdString()) >> m_objectDetector;
        qDebug() << "MaskDetection :: Dlib model loaded successfully.";

        QString writableDir =
            QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);

        QDir().mkpath(writableDir);

        QString targetPath = writableDir + "/test_image.jpeg";

        QFile::remove(targetPath);

        QFile resourceFile(":/test_image.jpeg");

        qDebug() << "MaskDetection :: Resource exists:"
                 << resourceFile.exists();

        if (resourceFile.exists() && resourceFile.copy(targetPath)) {
            qDebug() << "MaskDetection :: Test image copied to:" << targetPath;
            testWithImage(targetPath);
        }

    } catch (const std::exception &e) {
        qWarning() << "MaskDetection :: Failed to load model:" << e.what();
    }
}

MaskDetection::~MaskDetection()
{
}

bool MaskDetection::testWithImage(const QString &imagePath)
{
    cv::Mat bgrImage = cv::imread(imagePath.toStdString());

    if (bgrImage.empty()) {
        qWarning() << "MaskDetection :: Could not load test image.";
        return false;
    }

    cv::Mat rgbImage;
    cv::cvtColor(bgrImage, rgbImage, cv::COLOR_BGR2RGB);

    dlib::cv_image<dlib::rgb_pixel> dlibImg(rgbImage);

    std::vector<dlib::rectangle> rects =
        m_objectDetector(dlibImg);

    qDebug() << "MaskDetection :: Test image detected rectangles:"
             << rects.size();

    for (const auto &rect : rects) {
        cv::Rect cvRect = dlib::dlibRectangleToOpenCV(rect);

        qDebug() << "MaskDetection :: Rect:"
                 << cvRect.x
                 << cvRect.y
                 << cvRect.width
                 << cvRect.height;
    }

    return !rects.empty();
}

bool MaskDetection::executeObjDetector(cv::Mat &cameraInput)
{
    if (cameraInput.empty()) {
        qWarning() << "MaskDetection :: Empty input image.";
        return false;
    }

    cv::Mat bgrInput = cameraInput.clone();

    if (bgrInput.channels() == 4) {
        cv::cvtColor(bgrInput, bgrInput, cv::COLOR_BGRA2BGR);
    }
    else if (bgrInput.channels() == 1) {
        cv::cvtColor(bgrInput, bgrInput, cv::COLOR_GRAY2BGR);
    }
    else if (bgrInput.channels() != 3) {
        qWarning() << "MaskDetection :: Unsupported channels:"
                   << bgrInput.channels();
        return false;
    }

    qDebug() << "MaskDetection :: Input:"
             << bgrInput.cols << "x" << bgrInput.rows
             << "channels:" << bgrInput.channels();

    /*
     * IMPORTANT:
     * Eski kod 640x360 yapıyordu.
     * Bu, portrait frame'i yatay ezdiği için detector çalışmıyordu.
     * Burada aspect ratio korunuyor.
     */
    const int maxSide = 640;

    double resizeScale =
        static_cast<double>(maxSide) /
        static_cast<double>(std::max(bgrInput.cols, bgrInput.rows));

    int resizedWidth =
        static_cast<int>(bgrInput.cols * resizeScale);

    int resizedHeight =
        static_cast<int>(bgrInput.rows * resizeScale);

    if (resizedWidth <= 0 || resizedHeight <= 0) {
        qWarning() << "MaskDetection :: Invalid resized size.";
        return false;
    }

    cv::Mat resizedBgr;

    cv::resize(
        bgrInput,
        resizedBgr,
        cv::Size(resizedWidth, resizedHeight)
        );

    qDebug() << "MaskDetection :: Resized:"
             << resizedBgr.cols << "x" << resizedBgr.rows;

    cv::Mat resizedRgb;
    cv::cvtColor(resizedBgr, resizedRgb, cv::COLOR_BGR2RGB);

    dlib::cv_image<dlib::rgb_pixel> dlibImg(resizedRgb);

    std::vector<dlib::rectangle> detectedRectangles =
        m_objectDetector(dlibImg);

    qDebug() << "MaskDetection :: Detected rectangles:"
             << detectedRectangles.size();

    if (detectedRectangles.empty()) {
        return false;
    }

    double scaleX =
        static_cast<double>(bgrInput.cols) / resizedBgr.cols;

    double scaleY =
        static_cast<double>(bgrInput.rows) / resizedBgr.rows;

    for (const auto &rect : detectedRectangles) {
        cv::Rect faceRect =
            dlib::dlibRectangleToOpenCV(rect);

        if (faceRect.area() <= 5) {
            continue;
        }

        faceRect.x =
            static_cast<int>(faceRect.x * scaleX);

        faceRect.y =
            static_cast<int>(faceRect.y * scaleY);

        faceRect.width =
            static_cast<int>(faceRect.width * scaleX);

        faceRect.height =
            static_cast<int>(faceRect.height * scaleY);

        qDebug() << "MaskDetection :: Scaled rect:"
                 << faceRect.x
                 << faceRect.y
                 << faceRect.width
                 << faceRect.height;

        cv::Rect imageBounds(
            0,
            0,
            cameraInput.cols,
            cameraInput.rows
            );

        faceRect =
            faceRect & imageBounds;

        if (faceRect.width <= 0 || faceRect.height <= 0) {
            continue;
        }

        cv::rectangle(
            cameraInput,
            faceRect,
            cv::Scalar(0, 255, 0),
            4
            );

        return true;
    }

    return false;
}