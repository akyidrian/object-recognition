/** 
 * @file Timer.cpp
 * @author Aydin Arik 
 * @brief Provides profiling functionality for a section of code. 
 */

/******************************************************************************
 *                              Header Files
 ******************************************************************************/
#include "Timer.h"
#include <iostream>

// Size of time difference buffer. Buffer is used for finding the average time difference.
#define BUFF_SIZE 10 

using namespace std;


/******************************************************************************
 *                              Private Methods
 ******************************************************************************/

/**
 * Create a new timespec time structure and initialise data elements to zero.
 * 
 * @return A newly initialised timespec structure.
 */
timespec Timer::newTime() {
    timespec time;
    time.tv_nsec = 0;
    time.tv_sec = 0;

    return time;
}

/**
 * Reset startTime and endTime to zero. Like resetting a stop watch.
 */
void Timer::resetRecordedTimes() {
    startTime.tv_nsec = 0;
    startTime.tv_sec = 0;
    endTime.tv_nsec = 0;
    endTime.tv_sec = 0;
}

/**
 * Store a time difference in a buffer.
 * 
 * @param timeDiff A time difference.
 */
void Timer::storeTimeDiff(timespec timeDiff) {

    // Pop of the oldest time differences from the buffer. Ensure the size of the
    // buffer is of size BUFF_SIZE.0
    while (timeDiffHistory.size() >= BUFF_SIZE) {
        timeDiffHistory.pop_front();
    }
    
    timeDiffHistory.push_back(timeDiff); //Add new time difference to buffer.
}

/**
 * Averages time difference buffer.
 */
void Timer::averageTimeDiff() {
    timespec timeAvg = newTime();
    
    //Accumulate time difference stored in buffer.
    for (int i = 0; i < timeDiffHistory.size(); i++) {
        timeAvg.tv_nsec += timeDiffHistory.at(i).tv_nsec;
        timeAvg.tv_sec += timeDiffHistory.at(i).tv_sec;

    }
    
    //Divide by the number of buffer elements to get the average.
    timeAvg.tv_nsec = timeAvg.tv_nsec / timeDiffHistory.size();
    timeAvg.tv_sec = timeAvg.tv_sec / timeDiffHistory.size();

    timeDiffAvg = timeAvg;
}

/**
 * Calculating the time difference of two timespecs.
 * 
 * Code from:
 * http://www.guyrutenberg.com/2007/09/22/profiling-code-using-clock_gettime/
 * 
 * @param start Start Time.
 * @param end End Time.
 * @return Time Difference (end - start).
 */
timespec Timer::getTimeDiff(timespec start, timespec end) {
    timespec temp;
    
    //Checking whether there is a negative difference and account for it if there is.
    if ((end.tv_nsec - start.tv_nsec) < 0) { 
        temp.tv_sec = end.tv_sec - start.tv_sec - 1;
        temp.tv_nsec = 1000000000 + end.tv_nsec - start.tv_nsec; //Borrowing a second here.
    } 
    //Positive difference, hence don't need to account anything. Just take the difference.
    else {
        temp.tv_sec = end.tv_sec - start.tv_sec;
        temp.tv_nsec = end.tv_nsec - start.tv_nsec;
    }
    return temp;
}


Timer::Timer() {
    startTime = newTime();
    endTime = newTime();
    currTimeDiff = newTime();
    timeDiffAvg = newTime();
    getStartTime = true; //Must be true to record first time in startTime (see recordTime() method).
    timeDiffHistory.clear();
}

/******************************************************************************
 *                              Public Methods
 ******************************************************************************/
/**
 * Acts like a stop watch. First call to this will get a startTime, then a second
 * will the the endTime. When an endTime is gotten, a time difference is found
 * and stored.
 */
void Timer::recordTime() {

    if (getStartTime) {
        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &startTime);
        getStartTime = false; //get an endTime next call.
    } else {
        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &endTime);
        timespec timeDiff = getTimeDiff(startTime, endTime);
        currTimeDiff = timeDiff;
        storeTimeDiff(timeDiff);
        resetRecordedTimes();

        getStartTime = true; //get a startTime next call.
    }
}

/**
 * Get the most recently found time difference.
 * 
 * @return Most recent time difference.
 */
timespec Timer::getCurrTimeDiff() {
    return currTimeDiff;
}

/**
 * Get the time difference average. Will get the most up to date average.
 * 
 * @return Average time difference.
 */
timespec Timer::getTimeDiffAvg() {
    averageTimeDiff();
    return timeDiffAvg;
}
