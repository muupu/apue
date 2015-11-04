#include "apue.h"

#define	MAXARGC		50	
#define	WHITE	" \t\n"	

/*
 * 将字符串分解成标准argv型参数表，并调用用户函数处理参数
 */
int
buf_args(char *buf, int (*optfunc)(int, char **))
{
	char	*ptr, *argv[MAXARGC];
	int		argc;

	if (strtok(buf, WHITE) == NULL)		/* an argv[0] is required */
		return(-1);
	argv[argc = 0] = buf;
	while ((ptr = strtok(NULL, WHITE)) != NULL) { // 将字符串分割成独立的参数
		if (++argc >= MAXARGC-1)	/* -1 for room for NULL at end */
			return(-1);
		argv[argc] = ptr;
	}
	argv[++argc] = NULL;

	/*
	 * Since argv[] pointers point into the user's buf[],
	 * user's function can just copy the pointers, even
	 * though argv[] array will disappear on return.
	 */
	return((*optfunc)(argc, argv));
}
