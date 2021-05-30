#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>

#include "skeleton/fs.h"
#include "skeleton/logger.h"

int fs_write(const char *filename, void *element, int size)
{
	int rc;
	FILE *fp;

	if (filename)
	{
		fp = fopen(filename, "w");
		if (NULL == fp)
		{
			// fprintf(stdout, "[FATAL] fs init path not correct, check fs_init function on main \n");
			LOGGER_FATAL("fs init path not correct, check fs_init function on main \n");
			fflush(stdout);
		}
	}

	rc = fwrite(element, size, 1, fp);
	fclose(fp);

	return rc;
}

int fs_read(const char *filename, void *element, int size)
{
	int rc;
	FILE *fp;

	if (filename)
	{
		fp = fopen(filename, "r");
		if (NULL == fp)
		{
			// fprintf(stdout, "[FATAL] fs init path not correct, check fs_init function on main \n");
			LOGGER_WARNING("fs init path not correct, check fs_init function on main \n");
			fflush(stdout);
			return -1;
		}
	}

	while (!feof(fp))
	{
		fread(element, size, 1, fp);
	}

	fclose(fp);

	return 0;
}
