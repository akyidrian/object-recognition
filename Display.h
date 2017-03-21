/** 
 * @file Display.h
 * @author Aydin Arik 
 * @brief Creates an image to display for the object recognition system. The image
 *        shows the matches, highlights recognised objects and shows the frame rate. 
 */

#ifndef DISPLAY_H
#define	DISPLAY_H

/*******************************************************************************
 *                              Header Files
 ******************************************************************************/
#include <vector>
#include <opencv2/core/core.hpp>
#include <cstring>
#include "Object.h"
#include <time.h>

/*******************************************************************************
 *                              Class
 ******************************************************************************/
class Display {
private:
    cv::Mat displayImg;

    /**
     * Convert a floating point number to a string.
     * 
     * @param number Floating point number to convert.
     * @return String result.
     */
    std::string toString(float number);
public:

    /**
     * Call this to attach frame-rate information to an image that is to be displayed.
     * 
     * @param displayImg Image that is to be displayed.
     * @param timeDiff A time difference (relating to frame-rate) to be displayed.
     */
    void displayFPS(cv::Mat& imageMatches, timespec timeDiff);

    /**
     * Call this to display matching of keypoints and the identification of objects.
     * 
     * @param object Current object that is being looked for in a frame.
     * @param frame Video frame.
     * @param frameKeypoints Keypoints of the video frame.
     * @param matches Matches between the objects image and video frame.
     * @param displayImg Image to display results.
     */
    void displayMatching(Object object, // 1st image and its keypoints
            cv::Mat image2, std::vector<cv::KeyPoint> keypoints2, // 2nd image and its keypoints
            std::vector<cv::DMatch > matches, // the matches
            cv::Mat& imageMatches // the image produced
            );

    void draw(cv::Mat displayImg);
};

#endif	/* DISPLAY_H */
