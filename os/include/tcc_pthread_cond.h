#include <pthread.h>
#include <time.h> //for struct timespec
#ifdef __cplusplus
extern "C" {
#endif

extern int tcc_pthread_cond_init(	pthread_cond_t *cond,
						const pthread_condattr_t *attr);
extern int tcc_pthread_cond_timedwait(	pthread_cond_t *cond,
							pthread_mutex_t * mutex,
							const struct timespec *abstime);
#ifdef __cplusplus
}
#endif

