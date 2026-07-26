#include "timer.h"

#define NS_PER_SEC (double)1000000000

/**
 * @brief calculates elapsed time between 2 timespec structs. 
 *
 * @param start first time value
 * @param end  second time value
 * @return returns the elapsed time
 */

double elapsed_seconds(struct timespec start, struct timespec end)
{
  return ((start.tv_sec - end.tv_sec) + (start.tv_nsec - end.tv_nsec) / NS_PER_SEC);
  //TODO: understand the math here, Emily?
}