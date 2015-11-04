#include	"opend.h"
#include	<sys/select.h>

void
loop(void)
{
	int		i, n, maxfd, maxi, listenfd, clifd, nread;
	char	buf[MAXLINE];
	uid_t	uid;
	fd_set	rset, allset;

	FD_ZERO(&allset);

	/* 创建服务器进程与客户进程连接的端点 */
	if ((listenfd = serv_listen(CS_OPEN)) < 0)
		log_sys("serv_listen error");
	FD_SET(listenfd, &allset);
	maxfd = listenfd;
	maxi = -1;

	for ( ; ; ) {
		rset = allset;	/* rset gets modified each time around */
		if ((n = select(maxfd + 1, &rset, NULL, NULL, NULL)) < 0)
			log_sys("select error");

		if (FD_ISSET(listenfd, &rset)) {
			/* 描述符listenfd可以随时读取，用serv_accept接受新客户的请求 */
			if ((clifd = serv_accept(listenfd, &uid)) < 0)
				log_sys("serv_accept error: %d", clifd);
			i = client_add(clifd, uid);
			FD_SET(clifd, &allset);
			if (clifd > maxfd)
				maxfd = clifd;	/* max fd for select() */
			if (i > maxi)
				maxi = i;	/* max index in client[] array */
			log_msg("new connection: uid %d, fd %d", uid, clifd);
			continue;
		}

		for (i = 0; i <= maxi; i++) {	/* go through client[] array */
			if ((clifd = client[i].fd) < 0)
				continue;
			if (FD_ISSET(clifd, &rset)) {
				/*  */
				if ((nread = read(clifd, buf, MAXLINE)) < 0) {
					log_sys("read error on fd %d", clifd);
				} else if (nread == 0) {
					log_msg("closed: uid %d, fd %d",
					  client[i].uid, clifd);
					client_del(clifd);	/* read返回0（文件结束）：客户进程已经中止 */
					FD_CLR(clifd, &allset);
					close(clifd);
				} else {	/* read返回大于0：客户进程发送一个新请求 */
					handle_request(buf, nread, clifd, client[i].uid);
				}
			}
		}
	}
}
