#include	"opend.h"

/*
 * 由buf_args调用，用来验证客户进程发送的参数个数是否正确
 */
int
cli_args(int argc, char **argv)
{
	if (argc != 3 || strcmp(argv[0], CL_OPEN) != 0) {
		strcpy(errmsg, "usage: <pathname> <oflag>\n");
		return(-1);
	}
	pathname = argv[1];		/* save ptr to pathname to open */
	oflag = atoi(argv[2]);
	return(0);
}
