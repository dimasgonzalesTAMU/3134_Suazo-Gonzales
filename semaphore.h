/*
    File: semaphore.H

    Author: R. Bettati
            Department of Computer Science
            Texas A&M University
    Date  : 08/02/11

*/

#ifndef _semaphore_H_                   // include file only once
#define _semaphore_H_

/*--------------------------------------------------------------------------*/
/* DEFINES */
/*--------------------------------------------------------------------------*/

/* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

//#include <pthread.h>
#include <mutex>
#include <condition_variable>
#include "BoundedBuffer.h"

/*--------------------------------------------------------------------------*/
/* DATA STRUCTURES */
/*--------------------------------------------------------------------------*/

/* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* FORWARDS */
/*--------------------------------------------------------------------------*/

/* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* CLASS   S e m a p h o r e  */
/*--------------------------------------------------------------------------*/

class Semaphore {
private:
  /* -- INTERNAL DATA STRUCTURES
     You may need to change them to fit your implementation. */

  // int             value;
  // pthread_mutex_t m;
  // pthread_cond_t  c;
  BoundedBuffer<int>* bf_mp4;
  std::mutex the_mutex;
  std::condition_variable the_notfull_cvar;
  std::condition_variable the_notempty_cvar;

public:

  /* -- CONSTRUCTOR/DESTRUCTOR */

  Semaphore(int _sizeOfBuffer);

  ~Semaphore();

  /* -- SEMAPHORE OPERATIONS */

  int P(int _content);
  int V();
};


#endif
