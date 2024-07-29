#include "mask_detection.h"

#include <dlib/opencv/cv_image.h>
#include <dlib/opencv/to_open_cv.h>
#include <dlib/array2d.h>
#include <dlib/geometry.h>
#include <dlib/image_processing.h>
#include <dlib/image_processing/shape_predictor.h>
#include <dlib/image_processing/frontal_face_detector.h>

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/core/types.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <iostream>
#include <cmath>

#include <QDebug>



namespace dlib
{
inline cv::Rect dlibRectangleToOpenCV(dlib::rectangle r)
{
    return cv::Rect(cv::Point2i(r.left(), r.top()), cv::Point2i(r.right() + 1, r.bottom() + 1));
}

inline dlib::rectangle openCVRectToDlib(cv::Rect r)
{
    return dlib::rectangle((long)r.tl().x, (long)r.tl().y, (long)r.br().x - 1, (long)r.br().y - 1);
}
} // namespace dlib

MaskDetection::MaskDetection(const std::string maskDataPath)
    : m_maskDataPath(maskDataPath)
{

    // Deserialize train data
    dlib::deserialize(m_maskDataPath) >> m_objectDetector;
}

MaskDetection::~MaskDetection()
{}

bool MaskDetection::executeObjDetector(cv::Mat& cameraInput)
{
    // Convert cv::Mat to dlib image
    dlib::cv_image<dlib::rgb_pixel> dlib_img(cameraInput);

    std::vector<dlib::rectangle> detectedRectangles;
    detectedRectangles = m_objectDetector(dlib_img);

    std::vector<cv::Rect> detectedFaces;
    std::for_each(detectedRectangles.begin(), detectedRectangles.end(), [&](dlib::rectangle const& rect)
                                                                            -> void
                  {
                      cv::Rect cvRect = dlib::dlibRectangleToOpenCV(rect);
                      if(cvRect.area() > 5)
                      {
                          detectedFaces.emplace_back(cvRect);
                      }
                  });

    // Declaring empty variable for detected card.
    if(detectedFaces.size() > 0)
    {

        cv::Rect faceRect(detectedFaces.front());

        // check the box within the image plane
        if (0 <= faceRect.x
            && 0 <= faceRect.width
            && faceRect.x + faceRect.width <= cameraInput.cols
            && 0 <= faceRect.y
            && 0 <= faceRect.height
            && faceRect.y + faceRect.height <= cameraInput.rows)
        {

            // Draw RECTANGLE to image
            cv::rectangle(cameraInput,faceRect, cv::Scalar(0, 255, 0), 4);
            return true;
        }
    }
    return false;
}




