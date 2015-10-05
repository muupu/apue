#include "apue.h"
#include "error.h"
#include "pthread.h"

int main(void)
{
	int err;
	struct timespec tout; // 纳秒级别
	struct tm *tmp; // 日期和时间
	char buf[64];
	pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

	pthread_mutex_lock(&lock);
	printf("mutex is locked\n");
	clock_gettime(CLOCK_REALTIME, &tout); // 系统实时时间
	tmp = localtime(&tout.tv_sec); // 转换成时间日期表示法
	strftime(buf, sizeof(buf), "%r", tmp); // 格式化时间
	printf("current time is %s\n", buf);
	tout.tv_sec += 10;

	err = pthread_mutex_timedlock(&lock, &tout); // 等待10秒后仍然获取不到锁，返回错误码ETIMEDOUT
	clock_gettime(CLOCK_REALTIME, &tout);
	tmp = localtime(&tout.tv_sec);
	strftime(buf, sizeof(buf), "%r", tmp);
	printf("the time is now %s\n", buf);

	if (err == 0)
		printf("mutex locked again\n");
	else
		printf("can't lock mutex again:%s\n", strerror(err));
	exit(0);
}