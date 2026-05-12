#ifndef OPENCV_HELPER_H
#define OPENCV_HELPER_H

#include <opencv2/imgproc.hpp>
#include <opencv2/core.hpp>

#include <QImage>
#include <QVideoFrame>
#include <QVideoFrameFormat>

cv::Mat imageToMat8(const QImage &image);

void ensureC3(cv::Mat *mat);

QImage mat8ToImage(const cv::Mat &mat);

cv::Mat yuvFrameToMat8(QVideoFrame frame);

void mat8ToYuvFrame(const cv::Mat &mat, uchar *dst);

#endif // OPENCV_HELPER_H