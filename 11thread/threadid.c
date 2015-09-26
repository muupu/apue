#include "../include/apue.h"
#include <pthread.h>

pthread_t ntid;

void printids(const char *s)
{
	pid_t pid;
	pthread_t tid;

	pid = getpid(); // 进程ID
	tid = pthread_self(); // 当前线程ID
	printf("%s pid %lu tid %lu (0x%lx)\n", s, (unsigned long)pid, (unsigned long)tid, (unsigned long)tid);
}

void *thr_fn(void *arg)
{
	printids("new thread: ");
	return((void *)0);
}

int main(void)
{
	int err;
	err = pthread_create(&ntid, NULL, thr_fn, NULL);
	if (err != 0)
	{
		err_exit(err, "can't create thread");
	}
	printids("main thread: ");
	sleep(1); // 主线程不休眠，可能比新线程早退出，导致新线程没机会运行，整个进程就中止了。
	exit(0);
}