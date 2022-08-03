#include <pthread.h>

#ifdef __cplusplus
extern "C" {
#endif

extern void tcc_uithread_create(const char *name, void (*thread_func)(void*), void *arg, bool bJavaCall);

#ifdef __cplusplus
}
#endif

