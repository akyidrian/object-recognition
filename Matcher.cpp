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
 * @file Matcher.cpp
 * @author Robert Laganiere and Aydin Arik 
 * @brief Allows feature matching to be done between images. This is done using 
 *        SURF and Random Sample Consensus.
 */

/******************************************************************************
 *                              Header Files
 ******************************************************************************/
#include "Matcher.h"
#include <opencv2/features2d/features2d.hpp>
#include <calib3d/calib3d.hpp>
#include <iostream>

using namespace std;


/******************************************************************************
 *                              Private Methods
 ******************************************************************************/
/**
 * Clear matches for which NN ratio is greater than threshold return the number of 
 * removed points.
 * 
 * @param matches Feature matches between images.
 * @return Number of bad matches removed.
 */
int Matcher::ratioTest(std::vector<std::vector<cv::DMatch> >& matches) {

    int removed = 0;

    // for all matches
    for (std::vector<std::vector<cv::DMatch> >::iterator matchIterator = matches.begin();
            matchIterator != matches.end(); ++matchIterator) {

        // if 2 NN has been identified
        if (matchIterator->size() > 1) {

            // check distance ratio
            if ((*matchIterator)[0].distance / (*matchIterator)[1].distance > ratio) {

                matchIterator->clear(); // remove match
                removed++;
            }

        } else { // does not have 2 neighbours

            matchIterator->clear(); // remove match
            removed++;
        }
    }

    return removed;
}


/**
 * Insert symmetrical matches in symMatches vector.
 * 
 * @param objectImg
 * @param frame
 * @param symMatches
 */
void Matcher::symmetryTest(const std::vector<std::vector<cv::DMatch> >& objectImg,
        const std::vector<std::vector<cv::DMatch> >& frame,
        std::vector<cv::DMatch> & symMatches) {

    // for all matches image 1 -> image 2
    for (std::vector<std::vector<cv::DMatch> >::const_iterator matchIterator1 = objectImg.begin();
            matchIterator1 != objectImg.end(); ++matchIterator1) {

        if (matchIterator1->size() < 2) // ignore deleted matches 
            continue;

        // for all matches image 2 -> image 1
        for (std::vector<std::vector<cv::DMatch> >::const_iterator matchIterator2 = frame.begin();
                matchIterator2 != frame.end(); ++matchIterator2) {

            if (matchIterator2->size() < 2) // ignore deleted matches 
                continue;

            // Match symmetry test
            if ((*matchIterator1)[0].queryIdx == (*matchIterator2)[0].trainIdx &&
                    (*matchIterator2)[0].queryIdx == (*matchIterator1)[0].trainIdx) {

                // add symmetrical match
                symMatches.push_back(cv::DMatch((*matchIterator1)[0].queryIdx,
                        (*matchIterator1)[0].trainIdx,
                        (*matchIterator1)[0].distance));
                break; // next match in image 1 -> image 2
            }
        }
    }
}


/**
 * Identify good matches using RANSAC
 * 
 * @param matches
 * @param objectImgKeypoints
 * @param frameKeypoints
 * @param outMatches
 * @return 
 */
cv::Mat Matcher::ransacTest(const std::vector<cv::DMatch>& matches,
        const std::vector<cv::KeyPoint>& objectImgKeypoints,
        const std::vector<cv::KeyPoint>& frameKeypoints,
        std::vector<cv::DMatch>& outMatches) {

    // Convert keypoints into Point2f	
    std::vector<cv::Point2f> points1, points2;
    for (std::vector<cv::DMatch>::const_iterator it = matches.begin();
            it != matches.end(); ++it) {

        // Get the position of left keypoints
        float x = objectImgKeypoints[it->queryIdx].pt.x;
        float y = objectImgKeypoints[it->queryIdx].pt.y;
        points1.push_back(cv::Point2f(x, y));
        // Get the position of right keypoints
        x = frameKeypoints[it->trainIdx].pt.x;
        y = frameKeypoints[it->trainIdx].pt.y;
        points2.push_back(cv::Point2f(x, y));
    }

    // Compute F matrix using RANSAC
    cv::Mat fundemental;
    std::vector<uchar> inliers(points1.size(), 0);
    try {
        fundemental = cv::findFundamentalMat(
                cv::Mat(points1), cv::Mat(points2), // matching points
                inliers, // match status (inlier ou outlier)  
                CV_FM_RANSAC, // RANSAC method
                distance, // distance to epipolar line
                confidence); // confidence probability
    } catch (cv::Exception& ex) {
        //Do nothing...just assume no fundamental matrix can be found.
    }

    // extract the surviving (inliers) matches
    std::vector<uchar>::const_iterator itIn = inliers.begin();
    std::vector<cv::DMatch>::const_iterator itM = matches.begin();
    // for all matches
    for (; itIn != inliers.end(); ++itIn, ++itM) {

        if (*itIn) { // it is a valid match

            outMatches.push_back(*itM);
        }
    }

    std::cout << "Number of matched points (after cleaning): " << outMatches.size() << std::endl;

    if (refineF) {
        // The F matrix will be recomputed with all accepted matches

        // Convert keypoints into Point2f for final F computation	
        points1.clear();
        points2.clear();

        for (std::vector<cv::DMatch>::const_iterator it = outMatches.begin();
                it != outMatches.end(); ++it) {

            // Get the position of left keypoints
            float x = objectImgKeypoints[it->queryIdx].pt.x;
            float y = objectImgKeypoints[it->queryIdx].pt.y;
            points1.push_back(cv::Point2f(x, y));
            // Get the position of right keypoints
            x = frameKeypoints[it->trainIdx].pt.x;
            y = frameKeypoints[it->trainIdx].pt.y;
            points2.push_back(cv::Point2f(x, y));
        }

        try {
            // Compute 8-point F from all accepted matches
            fundemental = cv::findFundamentalMat(
                    cv::Mat(points1), cv::Mat(points2), // matching points
                    CV_FM_8POINT); // 8-point method
        } catch (cv::Exception& ex) {
            //Do nothing...just assume no fundamental matrix can be found if there are
            //no matches.
        }
    }

    return fundemental;
}



/******************************************************************************
 *                              Public Methods
 ******************************************************************************/
Matcher::Matcher() : ratio(0.65f), refineF(true), confidence(0.99), distance(3.0) {

    // SURF is the default feature
    detector = new cv::SurfFeatureDetector();
    extractor = new cv::SurfDescriptorExtractor();
}

// Set the feature detector
void Matcher::setFeatureDetector(cv::Ptr<cv::FeatureDetector>& detect) {

    detector = detect;
}

// Set descriptor extractor
void Matcher::setDescriptorExtractor(cv::Ptr<cv::DescriptorExtractor>& desc) {

    extractor = desc;
}

// Set the minimum distance to epipolar in RANSAC
void Matcher::setMinDistanceToEpipolar(double d) {

    distance = d;
}

// Set confidence level in RANSAC
void Matcher::setConfidenceLevel(double c) {

    confidence = c;
}

// Set the NN ratio
void Matcher::setRatio(float r) {

    ratio = r;
}

// if you want the F matrix to be recalculated
void Matcher::refineFundamental(bool flag) {

    refineF = flag;
}

/**
 * Match feature points using symmetry test and RANSAC.
 * 
 * @param object
 * @param frame
 * @param matches
 * @param frameKeypoints
 */
void Matcher::match(Object object, cv::Mat& frame, // input images 
        std::vector<cv::DMatch>& matches, // output matches and keypoints
        std::vector<cv::KeyPoint>& frameKeypoints) {


    // 1a. Detection of the SURF features
    detector->detect(frame, frameKeypoints);
    std::vector<cv::KeyPoint> objectImgKeypoint = object.getKeypoints();


    std::cout << "Number of SURF points (1): " << objectImgKeypoint.size() << std::endl;
    std::cout << "Number of SURF points (2): " << frameKeypoints.size() << std::endl;

    // 1b. Extraction of the SURF descriptors
    cv::Mat frameDescriptors;
    extractor->compute(frame, frameKeypoints, frameDescriptors);
    
    cv::Mat objectImgDesciptors = object.getDescriptors();
    
    std::cout << "descriptor matrix size: " << objectImgDesciptors.rows << " by " << objectImgDesciptors.cols << std::endl;

    // 2. Match the two image descriptors

    // Construction of the matcher 
    cv::BruteForceMatcher<cv::L2<float> > matcher;

    // from image 1 to image 2
    // based on k nearest neighbours (with k=2)
    std::vector<std::vector<cv::DMatch> > matches1;
    matcher.knnMatch(objectImgDesciptors, frameDescriptors,
            matches1, // vector of matches (up to 2 per entry) 
            2); // return 2 nearest neighbours

    // from image 2 to image 1
    // based on k nearest neighbours (with k=2)
    std::vector<std::vector<cv::DMatch> > matches2;
    matcher.knnMatch(frameDescriptors, objectImgDesciptors,
            matches2, // vector of matches (up to 2 per entry) 
            2); // return 2 nearest neighbours

    std::cout << "Number of matched points 1->2: " << matches1.size() << std::endl;
    std::cout << "Number of matched points 2->1: " << matches2.size() << std::endl;


    if (matches2.size() > 0) {
        // 3. Remove matches for which NN ratio is > than threshold

        // clean image 1 -> image 2 matches
        int removed = ratioTest(matches1);
        std::cout << "Number of matched points 1->2 (ratio test) : " << matches1.size() - removed << std::endl;
        // clean image 2 -> image 1 matches
        removed = ratioTest(matches2);
        std::cout << "Number of matched points 1->2 (ratio test) : " << matches2.size() - removed << std::endl;

        // 4. Remove non-symmetrical matches
        std::vector<cv::DMatch> symMatches;
        symmetryTest(matches1, matches2, symMatches);

        std::cout << "Number of matched points (symmetry test): " << symMatches.size() << std::endl;

        // 5. Validate matches using RANSAC
        cv::Mat fundemental = ransacTest(symMatches, objectImgKeypoint, frameKeypoints, matches); //TODO: reuse this?
    }
}
