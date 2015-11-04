#include	"open.h"
#include	<sys/uio.h>		/* struct iovec */

/*
 * Open the file by sending the "name" and "oflag" to the
 * connection server and reading a file descriptor back.
 */
int
csopen(char *name, int oflag)
{
	int				len;
	char			buf[10];
	struct iovec	iov[3];
	static int		csfd = { -1, -1 };

	if (csfd < 0) {	
		if ((csfd = cli_conn(CS_OPEN)) < 0) {
			err_ret("cli_conn error");
			return(-1);
		}
	}
	sprintf(buf, " %d", oflag);		/* oflag to ascii */
	iov[0].iov_base = CL_OPEN " ";		/* string concatenation */
	iov[0].iov_len  = strlen(CL_OPEN) + 1;
	iov[1].iov_base = name;
	iov[1].iov_len  = strlen(name);
	iov[2].iov_base = buf;
	iov[2].iov_len  = strlen(buf) + 1;	/* +1 for null at end of buf */
	len = iov[0].iov_len + iov[1].iov_len + iov[2].iov_len;
	if (writev(fd[0], &iov[0], 3) != len) { // 父进程将包含路径名和打开模式的请求发送给服务器进程
		err_ret("writev error");
		return(-1);
	}

	/* read descriptor, returned errors handled by write() 父进程调用recv_fd返回描述符或出错消息*/
	return(recv_fd(fd[0], write)); 
}