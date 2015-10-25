#include "apue.h"

/*
创建一个从父进程到子进程的管道
*/
int
main(void)
{
	int		n;
	int		fd[2];
	pid_t	pid;
	char	line[MAXLINE];

	if (pipe(fd) < 0)  /* 创建管道 */
		err_sys("pipe error");
	if ((pid = fork()) < 0) {
		err_sys("fork error");
	} else if (pid > 0) {		/* parent 关闭父进程的读端 */
		close(fd[0]);
		write(fd[1], "hello world\n", 12); /* 父进程写数据到管道 */
	} else {					/* child 关闭子进程的写端*/
		close(fd[1]);
		n = read(fd[0], line, MAXLINE); /* 子进程从管道读数据 */
		write(STDOUT_FILENO, line, n);
	}
	exit(0);
}
