#include "fileload.h"

void *file_contents(const char *filename){
	int  length = 0;
    FILE *f = fopen(filename, "r");
    void *buffer;
    if (!f) {
        fprintf(stderr, "Can't open %s for read\n", filename);
        return NULL;
    }
    fseek(f, 0, SEEK_END);
    length = ftell(f);
	rewind(f);
    buffer = (char*)malloc(sizeof(char) * (length+1));
    length = fread(buffer, sizeof(char), length, f);
    fclose(f);
    ((char*)buffer)[length] = '\0';

    return buffer;
}

