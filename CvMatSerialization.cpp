/** 
 * @file CvMatSerialization.h
 * @author Christoph Heindl and Aydin Arik 
 * @brief Provides functionality to serialize cv::Mat (OpenCV matrix) objects.
 * 
 * @url http://cheind.wordpress.com/2011/12/06/serialization-of-cvmat-objects-using-boost/
 * @url http://stackoverflow.com/questions/4170745/serializing-opencv-mat-vec3f
 */


/******************************************************************************
 *                              Header Files
 ******************************************************************************/
#include "CvMatSerialization.h"
#include <fstream>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/serialization/split_free.hpp>





/******************************************************************************
 *                              Functions
 ******************************************************************************/
BOOST_SERIALIZATION_SPLIT_FREE(cv::Mat);
namespace boost {
    namespace serialization {

        /**
         * Boost provided serialization support for cv::Mat.
         * 
         * @param ar Archive.
         * @param m Matrix to save.
         * @param version Class version.
         */
        template<class Archive>
        void save(Archive & ar, const cv::Mat& m, const unsigned int version) {
            size_t elem_size = m.elemSize();
            size_t elem_type = m.type();

            //Saving data member to archive.
            ar & m.cols;
            ar & m.rows;
            ar & elem_size;
            ar & elem_type;

            const size_t data_size = m.cols * m.rows * elem_size;
            ar & boost::serialization::make_array(m.ptr(), data_size);
        }

        /**  */
        /**
         * Boost provided deserialization support for cv::Mat.
         * 
         * @param ar Archive.
         * @param m Loaded matrix.
         * @param version Class version.
         */
        template<class Archive>
        void load(Archive & ar, cv::Mat& m, const unsigned int version) {
            int cols, rows;
            size_t elem_size, elem_type;

            //Loading data member from archive.
            ar & cols;
            ar & rows;
            ar & elem_size;
            ar & elem_type;

            m.create(rows, cols, elem_type);

            size_t data_size = m.cols * m.rows * elem_size;
            ar & boost::serialization::make_array(m.ptr(), data_size);
        }
    }
}

/**
 * Serialize a matrix into a binary file with a chosen name.
 * 
 * @param m Matrix to serialise.
 * @param filename Name of the generated binary file.
 */
void saveMat(cv::Mat& m, std::string filename) {
    std::ofstream ofs(filename.c_str());
    boost::archive::binary_oarchive oa(ofs);
    //boost::archive::text_oarchive oa(ofs);
    oa << m;
}

/**
 * Deserialize a matrix from a specified binary file.
 * 
 * @param m Deserialised matrix.
 * @param filename Binary file name.
 */
void loadMat(cv::Mat& m, std::string filename) {
    std::ifstream ifs(filename.c_str());
    boost::archive::binary_iarchive ia(ifs);
    //boost::archive::text_iarchive ia(ifs);
    ia >> m;

}