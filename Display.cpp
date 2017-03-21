/** 
 * @file Display.cpp
 * @author Aydin Arik 
 * @brief Creates an image to display for the object recognition system. The image
 *        shows the matches, highlights recognised objects and shows the frame rate. 
 */

/*******************************************************************************
 *                              Header Files
 ******************************************************************************/
#include "Display.h"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include "Timer.h"

using namespace std;

/*******************************************************************************
 *                              Methods
 ******************************************************************************/
/**
 * Convert a floating point number to a string.
 * 
 * @param number Floating point number to convert.
 * @return String result.
 */
std::string Display::toString(float number) {
    std::ostringstream buff;
    buff << number;
    return buff.str();
}

/**
 * Call this to attach frame-rate information to an image that is to be displayed.
 * 
 * @param displayImg Image that is to be displayed.
 * @param timeDiff A time difference (relating to frame-rate) to be displayed.
 */
void Display::displayFPS(cv::Mat& displayImg, timespec timeDiff) {
    float accurateFPS = (float) (1 / ((double) timeDiff.tv_nsec / (double) 1000000000));
    float roundedFPS = std::floor(accurateFPS * 100) / 100; //Rounding down since accuracy is not important.
    
    //Attach FPS text to image that is to be displayed.
    cv::putText(displayImg, toString(roundedFPS), cv::Point(640 * 2 - 75, 35), cv::FONT_HERSHEY_SIMPLEX, 0.7, cv::Scalar(0, 255, 0), 2, 8, false);
}

/**
 * Call this to display matching of keypoints and the identification of objects.
 * 
 * @param object Current object that is being looked for in a frame.
 * @param frame Video frame.
 * @param frameKeypoints Keypoints of the video frame.
 * @param matches Matches between the objects image and video frame.
 * @param displayImg Image to display results.
 */
void Display::displayMatching(Object object, 
        cv::Mat frame, std::vector<cv::KeyPoint> frameKeypoints, 
        std::vector<cv::DMatch > matches, 
        cv::Mat& displayImg) {

    std::vector<cv::KeyPoint> keypoints1 = object.getKeypoints();
    // draw the matches
    cv::drawMatches(object.getImage(), keypoints1, // 1st image and its keypoints
            frame, frameKeypoints, // 2nd image and its keypoints
            matches, // the matches
            displayImg, // the image produced
            cv::Scalar(255, 255, 255)); // color of the lines

    cv::Mat image1 = object.getImage();

    
    //Attempt to draw an outline of the object if one is found.
    try {
        //-- Localize the object from img_1 in img_2 
        cv::vector<cv::Point2f> obj;
        cv::vector<cv::Point2f> scene;

        for (int i = 0; i < matches.size(); i++) {
            //-- Get the keypoints from the good matches
            obj.push_back(keypoints1[ matches[i].queryIdx ].pt);
            scene.push_back(frameKeypoints[ matches[i].trainIdx ].pt);
        }

        cv::Mat H = cv::findHomography(obj, scene, CV_RANSAC);


        //-- Get the corners from the image_1 ( the object to be "detected" )
        cv::Point2f obj_corners[4] = {cvPoint(0, 0), cvPoint(image1.cols, 0), cvPoint(image1.cols, image1.rows), cvPoint(0, image1.rows)};
        cv::Point scene_corners[4];

        //-- Map these corners in the scene ( image_2)
        for (int i = 0; i < 4; i++) {
            double x = obj_corners[i].x;
            double y = obj_corners[i].y;

            double Z = 1. / (H.at<double>(2, 0) * x + H.at<double>(2, 1) * y + H.at<double>(2, 2));
            double X = (H.at<double>(0, 0) * x + H.at<double>(0, 1) * y + H.at<double>(0, 2)) * Z;
            double Y = (H.at<double>(1, 0) * x + H.at<double>(1, 1) * y + H.at<double>(1, 2)) * Z;
            scene_corners[i] = cvPoint(cvRound(X) + image1.cols, cvRound(Y));
        }

        if (matches.size() >= (0.05 * keypoints1.size())) { //Threshold for object recognition.
            cv::line(displayImg, scene_corners[0], scene_corners[1], cv::Scalar(0, 255, 0), 2);
            cv::line(displayImg, scene_corners[1], scene_corners[2], cv::Scalar(0, 255, 0), 2);
            cv::line(displayImg, scene_corners[2], scene_corners[3], cv::Scalar(0, 255, 0), 2);
            cv::line(displayImg, scene_corners[3], scene_corners[0], cv::Scalar(0, 255, 0), 2);

            const string name = object.getObjectName();

            cv::putText(displayImg, name, cv::Point(660, 30), cv::FONT_HERSHEY_SIMPLEX, 0.7, cv::Scalar(0, 255, 0), 2, 8, false);
        }


    } catch (cv::Exception& ex) {
        //Do nothing...This is required to prevent a crash due to no matches being found.
    }
}

/**
 * Display a specified image.
 * 
 * @param displayImg
 */
void Display::draw(cv::Mat displayImg) {
    this->displayImg = displayImg;
    cv::imshow("Object Recognition", displayImg);
}
