#ifndef MASK_DETECTION_H
#define MASK_DETECTION_H

#include <QString>

#include <opencv2/opencv.hpp>

#include <dlib/opencv.h>
#include <dlib/image_processing.h>
#include <dlib/image_processing/object_detector.h>
#include <dlib/image_processing/scan_fhog_pyramid.h>
#include <dlib/image_transforms/image_pyramid.h>
#include <dlib/serialize.h>

typedef dlib::scan_fhog_pyramid<dlib::pyramid_down<6>> image_scanner_type;

namespace dlib {
typedef object_detector<scan_fhog_pyramid<pyramid_down<6>>> simple_object_detector;
}

class MaskDetection
{
public:
    explicit MaskDetection(const QString &maskDataPath);
    ~MaskDetection();

    bool testWithImage(const QString &imagePath);
    bool executeObjDetector(cv::Mat &image);

private:
    QString m_maskDataPath;
    dlib::simple_object_detector m_objectDetector;
};

#endif // MASK_DETECTION_H