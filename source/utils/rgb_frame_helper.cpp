#include "rgb_frame_helper.h"

#include <QDebug>

QImage RGBHelper::imageWrapper(QVideoFrame frame)
{
    if (!frame.isValid()) {
        qWarning() << "imageWrapper: invalid video frame";
        return QImage();
    }

    QImage image = frame.toImage();

    if (image.isNull()) {
        qWarning() << "imageWrapper: could not convert QVideoFrame to QImage";
        return QImage();
    }

    return image;
}