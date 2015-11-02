#include "apue.h"
#include <sys/socket.h>
#include <sys/un.h>

int
main(void)
{
	int fd, size;
	struct sockaddr_un un;

	un.sun_family = AF_UNIX;
	strcpy(un.sun_path, "foo.socket");
	if ((fd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0)
		err_sys("socket failed");
	// 绑定地址长度：计算sun_path成员在sockaddr_un结构中的偏移量，然后与路径名长度相加
	size = offsetof(struct sockaddr_un, sun_path) + strlen(un.sun_path);
	if (bind(fd, (struct sockaddr *)&un, size) < 0)
		err_sys("bind failed");
	printf("UNIX domain socket bound\n");
	exit(0);
}
