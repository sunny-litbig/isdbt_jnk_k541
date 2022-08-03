#ifdef __cplusplus
extern "C" {
#endif

#include <pthread.h>
#include <time.h> //for struct timespec, CLOCK_MONOTONIC

int tcc_pthread_cond_init(pthread_cond_t *cond,
						const pthread_condattr_t *attr)
{
	int ret = 0;
#if defined(HAVE_LINUX_PLATFORM)
	pthread_condattr_t attribute;
	pthread_condattr_init(&attribute);
	pthread_condattr_setclock(&attribute, CLOCK_MONOTONIC);
	ret = pthread_cond_init(cond, &attribute);
	pthread_condattr_destroy(&attribute);
#endif
	return ret;
}

int tcc_pthread_cond_timedwait(pthread_cond_t *cond,
							pthread_mutex_t * mutex,
							const struct timespec *abstime)
{
	int ret = 0;
#if defined(HAVE_LINUX_PLATFORM)
	ret = pthread_cond_timedwait(cond, mutex, abstime);
#endif
	return ret;
}
#ifdef __cplusplus
}
#endif
