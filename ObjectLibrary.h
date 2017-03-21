/** 
 * @file ObjectLibrary.h
 * @author Aydin Arik 
 * @brief 
 */

#ifndef OBJECTLIBRARY_H
#define	OBJECTLIBRARY_H

/******************************************************************************
 *                              Header Files
 ******************************************************************************/
#include <cstring>
#include "Object.h"

/******************************************************************************
 *                              Class
 ******************************************************************************/
class ObjectLibrary {
private:
    int objectIterator; //Current object of interest in the library.
    int totalNumOfObjects;
    std::string libDirString; //Directory to search for object images.
    std::vector <Object> objects; //List (library) of objects.

    /**
     * Searches a specified folder for object images, then stores them. The filename 
     * is used as the name of the object. Uses the Boost Filesystem Library.
     * 
     * @authors Jeff Garland, Beman Dawes and Aydin Arik
     * @return Objects created sucessfully (0), not path specified was not found (1).
     */
    int createObjects();
public:
    ObjectLibrary();

    /**
     * Get the first object in the library.
     * 
     * @return The first object in library.
     */
    Object getFirst();

    /**
     * Get the next object in the library.
     * 
     * @return The next object in library.
     */
    Object getNextObject();

};



#endif	/* OBJECTLIBRARY_H */

