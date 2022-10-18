#include "util/file.h"

#include <al/malloc.h>

#include <stdio.h>

long ews_get_file_size(const char *fpath)
{
	long fsize;
	FILE *fp;

	fp = fopen(fpath, "r+");
	if (!fp)
		return -1;

	fseek(fp, 0, SEEK_END);
	fsize = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	fclose(fp);

	return fsize;
}

char *ews_get_file_content(const char *fpath)
{
	long fsize;
	char *content;
	FILE *fp;

	fp = fopen(fpath, "rb");
	if (!fp)
		return NULL;

	fseek(fp, 0, SEEK_END);
	fsize = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	content = al_malloc(fsize + 1);
	fread(content, 1, fsize, fp);
	fclose(fp);

	content[fsize] = 0;
	return content;
}
