#include "server/threadpool.h"

#include <al/malloc.h>

#include <malloc.h>

/* Threads will execute this function */
static void *thread_function(void *arg)
{
	struct ews_threadpool *threadpool = (struct ews_threadpool *)arg;

	while (threadpool->active) {
		pthread_mutex_lock(&threadpool->mutex);
		pthread_cond_wait(&threadpool->cond, &threadpool->mutex);

		struct ews_threadtask *task = al_queue_pop(threadpool->worker);

		pthread_mutex_unlock(&threadpool->mutex);

		if (task && task->task) {
			task->task(task->arg);
			free(task);
		}

		pthread_cond_signal(&threadpool->cond);
	}

	return NULL;
}

struct ews_threadpool *ews_threadpool_init(size_t nthreads)
{
	struct ews_threadpool *threadpool =
		al_malloc(sizeof(struct ews_threadpool));

	/* threadpool configuration */
	threadpool->nthreads = nthreads;
	threadpool->active = 1;
	threadpool->worker = al_queue_init();

	pthread_mutex_init(&threadpool->mutex, NULL);
	pthread_cond_init(&threadpool->cond, NULL);

	threadpool->pool = al_malloc(nthreads * sizeof(pthread_t));

	for (size_t i = 0; i < nthreads; i++)
		pthread_create(&threadpool->pool[i], NULL, thread_function,
			       threadpool);

	return threadpool;
}

void ews_threadpool_free(struct ews_threadpool *threadpool)
{
	if (!threadpool)
		return;

	threadpool->active = 0;

	for (size_t i = 0; i < threadpool->nthreads; i++)
		pthread_cond_signal(&threadpool->cond);

	for (size_t i = 0; i < threadpool->nthreads; i++)
		pthread_join(threadpool->pool[i], NULL);

	if (threadpool->pool)
		free(threadpool->pool);

	al_queue_free(threadpool->worker);
	pthread_mutex_destroy(&threadpool->mutex);
	pthread_cond_destroy(&threadpool->cond);
	free(threadpool);
}

struct ews_threadtask *ews_threadpool_createtask(void *(*fun)(void *arg),
						 void *arg)
{
	struct ews_threadtask *task = al_malloc(sizeof(struct ews_threadtask));
	task->task = fun;
	task->arg = arg;
	return task;
}

void ews_threadpool_addtask(struct ews_threadpool *threadpool,
			    struct ews_threadtask *task)
{
	pthread_mutex_lock(&threadpool->mutex);
	al_queue_push(threadpool->worker, task, sizeof(*task));
	pthread_cond_signal(&threadpool->cond);
	pthread_mutex_unlock(&threadpool->mutex);
}
