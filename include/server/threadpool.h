#ifndef _SERVER_THREADPOOL_H_
#define _SERVER_THREADPOOL_H_

#include <pthread.h>
#include <al/queue.h>

/* EWS THREADPOOL STRUCTURE
 * nthreads specifies max number of threads
 * active specifies whether the thread is active or not
 * pool specifies the thread ids
 * mutex is the mutex lock
 * cond is the condition variable
 * worker is the queue of workers
 */
struct ews_threadpool {
	size_t nthreads;
	int active;
	pthread_t *pool;
	pthread_mutex_t mutex;
	pthread_cond_t cond;
	struct al_queue *worker;
};

/* EWS THREADTASK STRUCTURE
 * threadtask holds thread functions
 * task is a pointer to specific function
 * arg is the arguments of task
 */
struct ews_threadtask {
	void *(*task)(void *arg);
	void *arg;
};

struct ews_threadpool *ews_threadpool_init(size_t nthreads);
void ews_threadpool_free(struct ews_threadpool *threadpool);
struct ews_threadtask *ews_threadpool_createtask(void *(*fun)(void *arg),
						 void *arg);
void ews_threadpool_addtask(struct ews_threadpool *threadpool,
			    struct ews_threadtask *task);

#endif
