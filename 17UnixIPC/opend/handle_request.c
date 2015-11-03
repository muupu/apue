#include	"opend.h"
#include	<fcntl.h>

void
handle_request(char *buf, int nread, int fd)
{
	int		newfd;

	if (buf[nread-1] != 0) {
		snprintf(errmsg, MAXLINE-1,
		  "request not null terminated: %*.*s\n", nread, nread, buf);
		send_err(fd, -1, errmsg);
		return;
	}
	if (buf_args(buf, cli_args) < 0) {	/* 将客户进程请求分解成标准argv型的参数表，调用cli_args处理客户进程的参数*/
		send_err(fd, -1, errmsg);
		return;
	}
	if ((newfd = open(pathname, oflag)) < 0) { // 打开相应文件
		snprintf(errmsg, MAXLINE-1, "can't open %s: %s\n", pathname,
		  strerror(errno));
		send_err(fd, -1, errmsg);
		return;
	}
	if (send_fd(fd, newfd) < 0)		/* 经由fd管道将描述符回送给客户进程*/
		err_sys("send_fd error");
	close(newfd);		/* we're done with descriptor */
}
