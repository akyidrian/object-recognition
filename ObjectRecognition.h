/** 
 * @file ObjectRecognition.h
 * @author Aydin Arik 
 * @brief Ties together displaying, timing (profiling) and object recognition 
 *        (finding objects) functionality.
 */

#ifndef OBJECTRECOGNITION_H
#define	OBJECTRECOGNITION_H


/******************************************************************************
 *                              Header Files
 ******************************************************************************/
#include "Timer.h"
#include "Object.h"
#include "Display.h"
#include "ObjectLibrary.h"
#include "Matcher.h"

/******************************************************************************
 *                              Class
 ******************************************************************************/
class ObjectRecognition {
private:
    Timer timer; //Used to get average time difference so that the frame-rate can be displayed.
    ObjectLibrary objects; //Library of known object that need to be found if seen.
    Display display; //Displays what the camera sees along with matches, FPS, etc.
    Object currObjectToLookFor; //The current object from the objects library we are looking for in video frame.
    Matcher matcher; //Finds physical similarities in image of object and video frame.
public:
    ObjectRecognition();

    /**
     * Run through and find matches in the (video) frame and current object image and 
     * display results.
     * 
     * @param frame Input frame from camera.
     * @param displayImg Image to display or save.
     * @return Success (1) or failure (0) to completely execute object recognition.
     */
    int run(cv::Mat frame, cv::Mat& displayImg);
};


#endif	/* OBJECTRECOGNITION_H */

