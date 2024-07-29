#ifndef MASK_DETECTION_H
#define MASK_DETECTION_H

#include <memory>
#include <string>

#include <opencv2/opencv.hpp>
#include <dlib/image_processing.h>
#include <dlib/image_processing/frontal_face_detector.h>
//#include <dlib/image_processing/shape_predictor.h>
#include <dlib/image_processing/object_detector.h>


#include <dlib/opencv.h>
#include <dlib/image_processing/scan_fhog_pyramid.h>
#include <dlib/image_transforms/image_pyramid_abstract.h>

#include "mask_result_collector.h"



/**
  * @brief The mask struct for storing points.
  */

typedef dlib::scan_fhog_pyramid<dlib::pyramid_down<6>> image_scanner_type;

namespace dlib{
typedef object_detector<scan_fhog_pyramid<pyramid_down<6>>> simple_object_detector;
}
struct mask
{
public:
    std::array<cv::Point, 6> eyeCoordinates {
        cv::Point(0,0),
        cv::Point(0,0),
        cv::Point(0,0),
        cv::Point(0,0),
        cv::Point(0,0),
        cv::Point(0,0)
    };
};


/**
  * \class LivenessDetection
  *
  * \brief Liveness Detection Class
  *
  *       Uses KYCEngine libraries and should be call as a api
  */
class MaskDetection
{
public:
    /**
      * @brief C-tor
      * @param maskDataPath = Data path of shape predictor model. ( File with .dat extension.)
      */
    MaskDetection(const std::string maskDataPath);

    /**
      * @brief D-tor
      */
    ~MaskDetection();

    /**
      * @brief maskResultCollector
      * @return
      */
    MaskResultCollector* maskResultCollector();

    /**
      * @brief executeObjDetectorDetection
      * @param Input frame from camera.
      * @param resizeFactor: between 0.1 - 1.0
      * @return
      */
    bool executeObjDetector(cv::Mat& image);

private:
    std::string m_maskDataPath = "";
    dlib::simple_object_detector m_objectDetector;


};



#endif // MASK_DETECTION_H
