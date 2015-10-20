#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <syslog.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <sys/stat.h>

#define LOCKFILE "/var/run/daemon.pid"
#define LOCKMODE (S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH)

extern int lockfile(int);

int
already_running(void)
{
	int		fd;
	char	buf[16];

	fd = open(LOCKFILE, O_RDWR|O_CREAT, LOCKMODE);
	if (fd < 0) {
		syslog(LOG_ERR, "can't open %s: %s", LOCKFILE, strerror(errno));
		exit(1);
	}
	if (lockfile(fd) < 0) { // 如果该文件已经加锁，lockfile会失败
		if (errno == EACCES || errno == EAGAIN) {
			close(fd);
			return(1); // 返回1，表示该守护进程已经在运行
		}
		syslog(LOG_ERR, "can't lock %s: %s", LOCKFILE, strerror(errno));
		exit(1);
	}
	ftruncate(fd, 0); // 需将文件长度截断为0。或者进程ID可能无法完全覆盖之前的进程ID字符串
	sprintf(buf, "%ld", (long)getpid()); // 将进程ID写进文件中，方便标识进程
	write(fd, buf, strlen(buf)+1);
	return(0);
}
