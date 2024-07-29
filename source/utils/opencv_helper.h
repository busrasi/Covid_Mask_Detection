#ifndef OPENCV_HELPER_H
#define OPENCV_HELPER_H

#include <opencv2/imgproc/imgproc.hpp>
#include <QImage>
#include <QVideoFrame>

// QImage -> CV_8UC4
cv::Mat imageToMat8(const QImage &image);

// CV_8UC3|4 -> CV_8UC3
void ensureC3(cv::Mat *mat);

// CV_8UC1|3|4 -> QImage
QImage mat8ToImage(const cv::Mat &mat);

// YUV QVideoFrame -> CV_8UC3
cv::Mat yuvFrameToMat8(const QVideoFrame &frame);

// CV_8UC3|4 -> YUV QVideoFrame
QVideoFrame mat8ToYuvFrame(const cv::Mat &mat);

// CV_8UC3|4 -> YUV pre-alloced mem
void mat8ToYuvFrame(const cv::Mat &mat, uchar *dst);

#endif // OPENCV_HELPER_H
