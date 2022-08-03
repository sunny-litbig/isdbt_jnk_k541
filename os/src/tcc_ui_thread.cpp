#include <tcc_ui_thread.h>

#if defined(HAVE_LINUX_PLATFORM)
#include <malloc.h>
#endif

struct tcc_ui_thread_handle {
	void (*thread_func)(void*);
	pthread_t pThread;
	void *arg;
};

void *tcc_ui_thread_func(void *arg)
{
	struct tcc_ui_thread_handle *handle = (struct tcc_ui_thread_handle*)arg;

	handle->thread_func(handle->arg);
	free(handle);

	return NULL;
}

void tcc_uithread_create(const char *name, void (*thread_func)(void*), void *arg, bool bJavaCall)
{
	struct tcc_ui_thread_handle *handle = (struct tcc_ui_thread_handle*)malloc(sizeof(struct tcc_ui_thread_handle));
	if(handle == NULL){
		return;
	}

	handle->arg = arg;
	handle->thread_func = thread_func;

	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

	pthread_create(&handle->pThread, &attr, tcc_ui_thread_func, handle);
	pthread_attr_destroy(&attr);
}
