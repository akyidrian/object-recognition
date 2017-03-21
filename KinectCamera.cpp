/** 
 * @file kinect.cpp
 * @author OpenKinect and Aydin Arik 
 * @brief Driver to Kinect camera. Allows the ability to get the cameras RGB and
 *        depth data.
 * 
 * @url http://openkinect.org/wiki/C%2B%2BOpenCvExample
 */


/******************************************************************************
 *                              Header Files
 ******************************************************************************/
#include <vector>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "KinectCamera.h"

using namespace cv;
using namespace std;


/******************************************************************************
 *                              Public Methods
 ******************************************************************************/

KinectCamera::KinectCamera(freenect_context *context, int index)
: Freenect::FreenectDevice(context, index),
bufferDepth(FREENECT_DEPTH_11BIT),
bufferRGB(FREENECT_VIDEO_RGB),
newRGBFrame(false), newDepthFrame(false),
depthMat(Size(640, 480), CV_16UC1),
RGBMat(Size(640, 480), CV_8UC3, Scalar(0)) {
}

/**
 * Get RGB data.
 * 
 * @param outputRGB
 * @return 
 */
bool KinectCamera::getVideo(Mat& outputRGB) {
    RGBMutex.lock();
    if (newRGBFrame) {//If there is a new RGB image, copy to outputRGB.
        cv::cvtColor(RGBMat, outputRGB, CV_RGB2BGR);
        newRGBFrame = false;
        RGBMutex.unlock();
        return true;
    } else {
        RGBMutex.unlock();
        return false;
    }
}

/**
 * Get depth data.
 * 
 * @param outputDepth
 * @return 
 */
bool KinectCamera::getDepth(Mat& outputDepth) {
    depthMutex.lock();
    if (newDepthFrame) { //If there is a new depth image, copy to outputDepth.
        depthMat.copyTo(outputDepth);
        newDepthFrame = false;
        depthMutex.unlock();
        return true;
    } else {
        depthMutex.unlock();
        return false;
    }
}



/******************************************************************************
 *                              Private Methods
 ******************************************************************************/

/**
 * Callback function for RGBData. Do not call this method directly.
 * 
 * @param RGBData
 * @param timestamp Time of callback.
 */
void KinectCamera::VideoCallback(void* RGBData, uint32_t timestamp) {
    RGBMutex.lock();
    RGBMat.data = static_cast<uint8_t*> (RGBData);
    newRGBFrame = true;
    RGBMutex.unlock();
};

/**
 * Callback function for depthData. Do not call this method directly.
 * 
 * @param depthData 
 * @param timestamp Time of callback.
 */
void KinectCamera::DepthCallback(void* depthData, uint32_t timestamp) {
    depthMutex.lock();
    depthMat.data = (uchar*) static_cast<uint16_t*> (depthData);
    newDepthFrame = true;
    depthMutex.unlock();
}

