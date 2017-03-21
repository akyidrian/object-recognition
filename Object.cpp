/** 
 * @file Object.cpp
 * @author Aydin Arik 
 * @brief Stores object data. This includes the objects name and image.
 *        Furthermore, keypoint and descriptors for the objects image are found 
 *        and store here so that comparisons to video frames can be done quickly
 *        when required.
 */

/******************************************************************************
 *                              Header Files
 ******************************************************************************/
#include "Object.h"
using namespace std;

/******************************************************************************
 *                              Methods
 ******************************************************************************/
Object::Object() {
    objectName = "null";
}

/**
 * Constructor.
 * 
 * @param objectName Name of object.
 * @param image Image of object.
 */
Object::Object(string objectName, cv::Mat image) {
    this->objectName = objectName;
    this->image = image;

    //
    //Find keypoints/ descriptors of image.
    //
    // pointer to the feature point detector object
    cv::Ptr<cv::FeatureDetector> detector = new cv::SurfFeatureDetector(1250);
    // pointer to the feature descriptor extractor object
    cv::Ptr<cv::DescriptorExtractor> extractor  = new cv::SurfDescriptorExtractor();

    detector->detect(this->image, keypoints);
    extractor->compute(image, keypoints, descriptors);
}

string Object::getObjectName() {
    return objectName;
}

cv::Mat Object::getImage() {
    return image;
}

vector<cv::KeyPoint> Object::getKeypoints() {
    return keypoints;
}

cv::Mat Object::getDescriptors() {
    return descriptors;
}
