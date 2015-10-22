#include "apue.h"
#include "error.h"
#include <errno.h>
#include <fcntl.h>

char	buf[500000];

int
main(void)
{
	int		ntowrite, nwrite;
	char	*ptr;

	ntowrite = read(STDIN_FILENO, buf, sizeof(buf));// 从标准输入500000字节
	fprintf(stderr, "read %d bytes\n", ntowrite);

	set_fl(STDOUT_FILENO, O_NONBLOCK);	/* set nonblocking 设置标准输出为非阻塞*/

	ptr = buf;
	while (ntowrite > 0) { // 轮询（会浪费cpu时间）
		errno = 0;
		nwrite = write(STDOUT_FILENO, ptr, ntowrite); //若write不能完成，立即出错返回（即非阻塞）
		fprintf(stderr, "nwrite = %d, errno = %d\n", nwrite, errno);

		if (nwrite > 0) {
			ptr += nwrite;
			ntowrite -= nwrite;
		}
	}

	clr_fl(STDOUT_FILENO, O_NONBLOCK);	/* clear nonblocking */

	exit(0);
}
