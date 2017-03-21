/** 
 * @file ObjectLibrary.cpp
 * @author Aydin Arik 
 * @brief A library of objects to easily scan over when trying to find an object
 *        in a (video) frame.
 */


/******************************************************************************
 *                              Header Files
 ******************************************************************************/
#include "ObjectLibrary.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "boost/filesystem/operations.hpp"
#include "boost/filesystem/path.hpp"
#include "boost/progress.hpp"
#include <iostream>//todo

using namespace cv;
using namespace std;
namespace fs = boost::filesystem;

#define BOOST_FILESYSTEM_VERSION 3

//  As an example program, we don't want to use any deprecated features
#ifndef BOOST_FILESYSTEM_NO_DEPRECATED 
#define BOOST_FILESYSTEM_NO_DEPRECATED
#endif
#ifndef BOOST_SYSTEM_NO_DEPRECATED 
#define BOOST_SYSTEM_NO_DEPRECATED
#endif


/******************************************************************************
 *                              Function
 ******************************************************************************/
template <class T> double sizeOf(T& array) {
    return (sizeof (array) / sizeof (array[0]));
}


/******************************************************************************
 *                              Methods
 ******************************************************************************/
/**
 * Get the next object in the library.
 * 
 * @return The next object in library.
 */
Object ObjectLibrary::getNextObject() {
    Object anObject = objects.at(objectIterator);

    
    if (objectIterator == (objects.size() - 1)) { //Start at the start of library if we have reached the end.
        objectIterator = 0;
    } else { //Iterate to next object in library.
        objectIterator++;
    }

    return anObject;
}

/**
 * Get the first object in the library.
 * 
 * @return The first object in library.
 */
Object ObjectLibrary::getFirst() {
    return objects.at(0);
}

ObjectLibrary::ObjectLibrary() {
    objectIterator = 0;
    totalNumOfObjects = 0;
    
    //Library folder.
    libDirString = "../../Images/Objects/";

    createObjects();
}


/**
 * Searches a specified folder for object images, then stores them. The filename 
 * is used as the name of the object. Uses the Boost Filesystem Library.
 * 
 * @authors Jeff Garland, Beman Dawes and Aydin Arik
 * @return Objects created sucessfully (0), not path specified was not found (1).
 */
int ObjectLibrary::createObjects() {
    fs::path path(libDirString);
    string extArr[] = {".jpg", ".png", ".bmp", ".tiff"}; //More files could be added if necessary.


    unsigned long file_count = 0;
    unsigned long dir_count = 0;
    if (!fs::exists(path)) {
        std::cout << "\nNot found: " << path << std::endl;
        return 1;
    }

    if (fs::is_directory(path)) { //Is the given path a directory?
        fs::directory_iterator end_iter;
        for (fs::directory_iterator dir_itr(path); dir_itr != end_iter; ++dir_itr) { //Iterate through directory.
            try {
                if (fs::is_directory(dir_itr->status())) { //Is the current directory iterator a directory also?
                    ++dir_count;
                    std::cout << dir_itr->path().filename() << " [directory]\n";
                } else if (fs::is_regular_file(dir_itr->status())) { //Is the current directory iterator a file?
                    
                    //Check file in directory is a valid image file.
                    for (int i = 0; i < sizeOf(extArr); i++) { 
                        //If it is a valid image file, then create an 'Object' object and place this in the library.
                        if (dir_itr->path().extension() == extArr[i]) { 
                            string fileName = dir_itr->path().stem().string();
                            objects.push_back(*(new Object(fileName, cv::imread(dir_itr->path().string(), CV_LOAD_IMAGE_GRAYSCALE))));
                            ++file_count;
                            totalNumOfObjects = file_count;
                        }
                    }
                }

            } catch (const std::exception & ex) {
                //Do nothing and ignore error.
            }
        }
//        std::cout << "\n" << file_count << " files\n"
//                << dir_count << " directories\n";
    } else //TODO: Must be a file
    {
        std::cout << "\nFound: " << path << "\n"; //file here    
    }
    
    return 0;
}

