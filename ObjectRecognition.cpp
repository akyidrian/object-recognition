/** 
 * @file ObjectRecognition.cpp
 * @author Aydin Arik 
 * @brief Ties together displaying, timing (profiling) and object recognition 
 *        (finding objects) functionality.
 */

/******************************************************************************
 *                              Header Files
 ******************************************************************************/
#include "ObjectRecognition.h"


/******************************************************************************
 *                              Methods
 ******************************************************************************/
ObjectRecognition::ObjectRecognition() {
    currObjectToLookFor = objects.getFirst();
    
    // Prepare the matcher
    matcher.setConfidenceLevel(0.85);
    matcher.setMinDistanceToEpipolar(3.0);
    matcher.setRatio(1.5f);
    matcher.refineFundamental(true);
    cv::Ptr<cv::FeatureDetector> pfd = new cv::SurfFeatureDetector(1250);
    matcher.setFeatureDetector(pfd);
}

/**
 * Run through and find matches in the (video) frame and current object image and 
 * display results.
 * 
 * @param frame Input frame from camera.
 * @param displayImg Image to display or save.
 * @return Success (1) or failure (0) to completely execute object recognition.
 */
int ObjectRecognition::run(cv::Mat frame, cv::Mat& displayImg) {
    
    timer.recordTime(); //Start profiling.
    
    currObjectToLookFor = objects.getNextObject();
    cv::Mat currObjImg = currObjectToLookFor.getImage();
    
    //Double checking to see there is image data. This should never be entered 
    //unless initialisation of camera and ObjectLibrary isn't done.
    if (!currObjImg.data || !frame.data)
        return 0; //Failure to process further in object recognition code.

    //Aids in matching and drawing...
    std::vector<cv::DMatch> matches;
    std::vector<cv::KeyPoint> frameKeypoints;

    //Finds physical similarities in image of object and video frame.
    matcher.match(currObjectToLookFor, frame, matches, frameKeypoints);

    timer.recordTime(); //End profiling.
    
    display.displayMatching(currObjectToLookFor, frame, frameKeypoints, matches, displayImg);
    display.displayFPS(displayImg, timer.getTimeDiffAvg());
 
    display.draw(displayImg);
    
    
    
    return 1; //Success.
}