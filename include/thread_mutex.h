
#ifndef __THREAD_MUTEX_H___
#define __THREAD_MUTEX_H___

#include "portable.h"
#include "return.h"

#ifdef _WIN32
#include <windows.h>
#else
#include <pthread.h>
#endif

#define SUPPORT_PTHREAD_MUTEX

#ifdef __cplusplus
extern "C"
{
#endif

	typedef void (*MUTEX_LOCK)(void *);
	typedef void (*MUTEX_UNLOCK)(void *);
	typedef void (*MUTEX_DESTROY)(void *);

	typedef struct tagTHREAD_MUTEX
	{
		MUTEX_LOCK lock;
		MUTEX_UNLOCK unlock;
		MUTEX_DESTROY destroy;

#ifdef _WIN32
		CRITICAL_SECTION mutex;
#else
	pthread_mutex_t mutex;
#endif

	} THREAD_MUTEX, *PTHREAD_MUTEX;

	/** 
 * @brief �̻߳�����
 *
 */
	typedef struct tagTHREAD_MUTEX *MUTEXHanle;

	MUTEXHanle thread_mutex_create(void);
	void thread_mutex_destroy(MUTEXHanle handle);

#define THREAD_MUTEX_LOCK(handle) \
	{                             \
		handle->lock(handle);     \
	}
#define THREAD_MUTEX_UNLOCK(handle) \
	{                               \
		handle->unlock(handle);     \
	}

#ifdef __cplusplus
}
#endif

#endif /// __THREAD_MUTEX_H___
