/** 
 * @file CvMatSerialization.h
 * @author Christoph Heindl and Aydin Arik 
 * @brief Provides functionality to serialize cv::Mat (OpenCV matrix) objects.
 * 
 * @url http://cheind.wordpress.com/2011/12/06/serialization-of-cvmat-objects-using-boost/
 * @url http://stackoverflow.com/questions/4170745/serializing-opencv-mat-vec3f
 */

#ifndef CVMAT_SERIALIZATION_H
#define	CVMAT_SERIALIZATION_H

/******************************************************************************
 *                              Header Files
 ******************************************************************************/
#include <opencv2/opencv.hpp>

/******************************************************************************
 *                              Functions
 ******************************************************************************/
/**
 * Serialize a matrix into a binary file with a chosen name.
 * 
 * @param m Matrix to serialise.
 * @param filename Name of the generated binary file.
 */
void saveMat(cv::Mat& m, std::string filename);

/**
 * Deserialize a matrix from a specified binary file.
 * 
 * @param m Deserialised matrix.
 * @param filename Binary file name.
 */
void loadMat(cv::Mat& m, std::string filename);

#endif	/* CVMAT_SERIALIZATION_H */

