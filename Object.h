/** 
 * @file Object.h
 * @author Aydin Arik 
 * @brief Stores object data. This includes the objects name and image.
 *        Furthermore, keypoint and descriptors for the objects image are found 
 *        and store here so that comparisons to video frames can be done quickly
 *        when required.
 */

#ifndef OBJECT_H
#define	OBJECT_H
#include <opencv2/features2d/features2d.hpp>

/******************************************************************************
 *                              Header Files
 ******************************************************************************/
#include <cstring>
#include <opencv2/core/core.hpp>

/******************************************************************************
 *                              Class
 ******************************************************************************/
class Object {
private:
    std::string objectName;
    cv::Mat image; //Image of the object.
    std::vector<cv::KeyPoint> keypoints;
    cv::Mat descriptors;

public:
    Object();

    /**
     * Constructor.
     * 
     * @param objectName Name of object.
     * @param image Image of object.
     */
    Object(std::string objectName, cv::Mat image);

    std::string getObjectName();

    cv::Mat getImage();

    std::vector<cv::KeyPoint> getKeypoints();

    cv::Mat getDescriptors();
};

#endif	/* OBJECT_H */

