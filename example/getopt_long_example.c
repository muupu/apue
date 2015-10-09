#include <stdio.h>
#include <getopt.h>

char *l_opt_arg;
char* const opt_string = "nbl:";

struct option long_options[] = {
	{ "name",    0,  NULL, 'a'},
	{ "bf_name", 0,  NULL, 'b'},
	{ "love",    1,  NULL, 'l'},
	{ NULL,      0,  NULL,  0 },
};

int main(int argc, char *argv[])
{
	int c;
	while((c = getopt_long (argc, argv, opt_string, long_options, NULL)) != -1)
	{
		switch (c)
		{
			case 'a':
				printf("My name is XL.\n");
				break;
			case 'b':
				printf("His name is ST.\n");
				break;
			case 'l':
				l_opt_arg = optarg;
				printf("Our love is %s!\n", l_opt_arg);
				break;
		}
	}
	return 0;
}