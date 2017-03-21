/** 
 * @file Timer.h
 * @author Aydin Arik 
 * @brief Provides profiling functionality for a section of code. Acts as a stop 
 *        watch which can take averages.
 */


#ifndef TIMER_H
#define	TIMER_H

/******************************************************************************
 *                              Header Files
 ******************************************************************************/
#include <deque>
#include <time.h>


/******************************************************************************
 *                              Class
 ******************************************************************************/
class Timer {
private:
    bool getStartTime; //Flag to decide whether a startTime or endTime should be gotten.
    timespec startTime;
    timespec endTime;
    timespec currTimeDiff; //Most recent time difference found.
    timespec timeDiffAvg; //Last average time difference found.
    std::deque<timespec> timeDiffHistory; //A buffer of time differences.

    /**
     * Reset startTime and endTime to zero. Like resetting a stop watch.
     */
    void resetRecordedTimes();

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
    timespec getTimeDiff(timespec start, timespec end);

    /**
     * Store a time difference in a buffer.
     * 
     * @param timeDiff A time difference.
     */
    void storeTimeDiff(timespec timeDiff);

    /**
     * Averages time difference buffer.
     */
    void averageTimeDiff();

    /**
     * Create a new timespec time structure and initialise data elements to zero.
     * 
     * @return A newly initialised timespec structure.
     */
    timespec newTime();
public:
    Timer();

    /**
     * Acts like a stop watch. First call to this will get a startTime, then a second
     * will the the endTime. When an endTime is gotten, a time difference is found
     * and stored.
     */
    void recordTime();

    /**
     * Get the most recently found time difference.
     * 
     * @return Most recent time difference.
     */
    timespec getCurrTimeDiff();

    /**
     * Get the time difference average. Will get the most up to date average.
     * 
     * @return Average time difference.
     */
    timespec getTimeDiffAvg();
};

#endif	/* TIMER_H */

