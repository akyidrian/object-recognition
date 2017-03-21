/*------------------------------------------------------------------------------------------*\
   This file contains material supporting chapter 9 of the cookbook:  
   Computer Vision Programming using the OpenCV Library. 
   by Robert Laganiere, Packt Publishing, 2011.

   This program is free software; permission is hereby granted to use, copy, modify, 
   and distribute this source code, or portions thereof, for any purpose, without fee, 
   subject to the restriction that the copyright notice may not be removed 
   or altered from any source or altered source distribution. 
   The software is released on an as-is basis and without any warranties of any kind. 
   In particular, the software is not guaranteed to be fault-tolerant or free from failure. 
   The author disclaims all warranties with regard to this software, any use, 
   and any consequent failure, is purely the responsibility of the user.
 
   Copyright (C) 2010-2011 Robert Laganiere, www.laganiere.name
\*------------------------------------------------------------------------------------------*/
/** 
 * @file Matcher.h
 * @author Robert Laganiere and Aydin Arik 
 * @brief Allows feature matching to be done between images. This is done using 
 *        SURF and Random Sample Consensus.
 */


#ifndef MATCHER_H
#define	MATCHER_H

/******************************************************************************
 *                              Header Files
 ******************************************************************************/
#include <string>
#include <vector>
#include <opencv2/core/core.hpp>
#include "Object.h"


/******************************************************************************
 *                              Class
 ******************************************************************************/
class Matcher {
private:

    // pointer to the feature point detector object
    cv::Ptr<cv::FeatureDetector> detector;
    // pointer to the feature descriptor extractor object
    cv::Ptr<cv::DescriptorExtractor> extractor;
    float ratio; // max ratio between 1st and 2nd NN
    bool refineF; // if true will refine the F matrix
    double distance; // min distance to epipolar
    double confidence; // confidence level (probability)

    
    int ratioTest(std::vector<std::vector<cv::DMatch> >& matches);

    // Insert symmetrical matches in symMatches vector

    void symmetryTest(const std::vector<std::vector<cv::DMatch> >& matches1,
            const std::vector<std::vector<cv::DMatch> >& matches2,
            std::vector<cv::DMatch> & symMatches);

    // Identify good matches using RANSAC
    // Return fundemental matrix

    cv::Mat ransacTest(const std::vector<cv::DMatch>& matches,
            const std::vector<cv::KeyPoint>& keypoints1,
            const std::vector<cv::KeyPoint>& keypoints2,
            std::vector<cv::DMatch>& outMatches);
    
    
public:

    Matcher();

    // Set the feature detector

    void setFeatureDetector(cv::Ptr<cv::FeatureDetector>& detect);

    // Set descriptor extractor

    void setDescriptorExtractor(cv::Ptr<cv::DescriptorExtractor>& desc);

    // Set the minimum distance to epipolar in RANSAC

    void setMinDistanceToEpipolar(double d);

    // Set confidence level in RANSAC

    void setConfidenceLevel(double c);

    // Set the NN ratio

    void setRatio(float r);

    // if you want the F matrix to be recalculated

    void refineFundamental(bool flag);

    // Clear matches for which NN ratio is > than threshold
    // return the number of removed points 
    // (corresponding entries being cleared, i.e. size will be 0)



    // Match feature points using symmetry test and RANSAC
    // returns fundemental matrix

    void match(Object object, cv::Mat& image2, // input images 
            std::vector<cv::DMatch>& matches, // output matches and keypoints
            std::vector<cv::KeyPoint>& keypoints2);

};

#endif	/* MATCHER_H */
