#ifndef RGB_FRAME_HELPER_H
#define RGB_FRAME_HELPER_H

#include <QImage>
#include <QVideoFrame>
#include <QVideoFrameFormat>

class RGBHelper
{
public:
    RGBHelper() = default;

    static QImage imageWrapper(QVideoFrame frame);
};

#endif // RGB_FRAME_HELPER_H