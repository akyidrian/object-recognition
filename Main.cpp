/** 
 * @file Main.h
 * @author Aydin Arik 
 * @brief 
 */


/******************************************************************************
 *                              Header Files
 ******************************************************************************/
#include <vector>
#include <opencv2/core/core.hpp>
#include "KinectCamera.h"
#include <cstring>
#include "ObjectRecognition.h"
#include <highgui/highgui.hpp>

using namespace std;

/**
 * 
 * @param argc
 * @param argv
 * @return 
 */
int main(int argc, char **argv) {
    /* Snapshot related variables */
    string filename("../../Images/Snapshots/snapshot");
    string suffix(".png");
    int snapCount(0);
    char keyPressed = -1;


    cv::Mat frame;
    bool stop(false);

    // Delay between each frame
    // corresponds to video frame rate
    int delay = 1000 / 30; //30hz

    Freenect::Freenect freenect;
    KinectCamera& device = freenect.createDevice< KinectCamera > (0);

    device.startVideo();

    ObjectRecognition recognition;
    cv::Mat image;
    // for all frames in video
    while (!stop) {
        device.getVideo(frame);

        recognition.run(frame, image);

        keyPressed = cv::waitKey(delay);
        if (keyPressed == 'q') {//any key
            stop = true;
        } else if (keyPressed == 's') {
            std::ostringstream file;
            file << filename << snapCount << suffix;
            cv::imwrite(file.str(), image);
            snapCount++;
        }
    }

    device.stopVideo();
    return 0;
}
