/** 
 * @file kinect.h
 * @author OpenKinect and Aydin Arik 
 * @brief Driver to Kinect camera. Allows the ability to get the cameras RGB and
 *        depth data.
 * 
 * @url http://openkinect.org/wiki/C%2B%2BOpenCvExample
 */

#ifndef KINECT_H
#define	KINECT_H


/******************************************************************************
 *                              Header Files
 ******************************************************************************/
#include "libfreenect.hpp"
#include <opencv2/core/core.hpp>
#include <pthread.h>


/******************************************************************************
 *                              Class
 ******************************************************************************/
/**
 * Mutexes required by the KinectCamera class.
 */
class Mutex {
public:
    
    Mutex() {
        pthread_mutex_init(&m_mutex, NULL);
    }
    
    void lock() {
        pthread_mutex_lock(&m_mutex);
    }
    
    void unlock() {
        pthread_mutex_unlock(&m_mutex);
    }
private:
    pthread_mutex_t m_mutex;
};


class KinectCamera : public Freenect::FreenectDevice {
private:
    std::vector<uint8_t> bufferDepth;
    std::vector<uint8_t> bufferRGB;
    cv::Mat depthMat;
    cv::Mat RGBMat;
    Mutex RGBMutex;
    Mutex depthMutex;
    bool newRGBFrame;
    bool newDepthFrame;

    // Do not call directly even in child
    void VideoCallback(void* _rgb, uint32_t timestamp);

    // Do not call directly even in child
    void DepthCallback(void* _depth, uint32_t timestamp);
    
public:
    /**
     * 
     * @param context
     * @param index
     */
    KinectCamera(freenect_context *context, int index);
    
    /**
     * 
     * @param outputRGB
     * @return 
     */
    bool getVideo(cv::Mat& outputRGB);
    
    /**
     * 
     * @param outputDepth
     * @return 
     */
    bool getDepth(cv::Mat& outputDepth);
};


#endif	/* KINECT_H */

