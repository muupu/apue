#include "apue.h"
#include <ctype.h>
#include <fcntl.h>

#define BSZ 4096

unsigned char buf[BSZ];

unsigned char
translate(unsigned char c)
{
	if (isalpha(c)) {
		if (c >= 'n')
			c -= 13;
		else if (c >= 'a')
			c += 13;
		else if (c >= 'N')
			c -= 13;
		else
			c += 13;
	}
	return(c);
}

int
main(int argc, char* argv[])
{
	int	ifd, ofd, i, n, nw;

	if (argc != 3)
		err_quit("usage: rot13 infile outfile");
	if ((ifd = open(argv[1], O_RDONLY)) < 0) // 打开输入文件
		err_sys("can't open %s", argv[1]);   // 打开输出文件
	if ((ofd = open(argv[2], O_RDWR|O_CREAT|O_TRUNC, FILE_MODE)) < 0)
		err_sys("can't create %s", argv[2]);

	while ((n = read(ifd, buf, BSZ)) > 0) { // 从输入文件中取出一个块
		for (i = 0; i < n; i++)
			buf[i] = translate(buf[i]); // 翻译字符
		if ((nw = write(ofd, buf, n)) != n) { // 写入输出文件中
			if (nw < 0)
				err_sys("write failed");
			else
				err_quit("short write (%d/%d)", nw, n);
		}
	}

	fsync(ofd);
	exit(0);
}
