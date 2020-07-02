#include "model.h"
#include "render.h"
#include "../utility/fileload.h"
#include <byteswap.h>
#include "../utility/objload.h"

int modelInitGL(model * m, GLint draw_type){
	int err = 0;

	glGenVertexArrays(1, 	&(m->VAO));
	glGenBuffers(1, 		&(m->VBO));
	glGenBuffers(1, 		&(m->EBO));

	glBindVertexArray(m->VAO);

	glBindBuffer(GL_ARRAY_BUFFER, m->VBO);
	glBufferData(GL_ARRAY_BUFFER, m->verts.size * sizeof(GLfloat), (GLfloat*)m->verts.data, draw_type);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m->EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m->indices.size * sizeof(GLuint), (GLfloat*)m->indices.data, draw_type);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(GLfloat), (void*)(0*sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8*sizeof(GLfloat), (void*)(3*sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8*sizeof(GLfloat), (void*)(5*sizeof(GLfloat)));
	glBindVertexArray(0);

	return err;
}
int modelInitTex(model * m, char * filename){
	int err = 0;

	unsigned char png_hdr[] = {137, 80, 78, 71, 13, 10, 26, 10};
	char* buffer = (char*)file_contents(filename);
	if(buffer == NULL) return -1;
	if(memcmp(buffer, png_hdr, 8)){
		printf("%s is not a png or is corrupt!\n", filename);
		return -1;
	}
	uint32_t width	= 0;
	uint32_t height	= 0;
	memcpy(&width, buffer+16, 4);
	memcpy(&height, buffer+20, 4);
	free(buffer);
	width = __bswap_32 (width);
	height = __bswap_32 (height);
//	printf("W: %d H: %d\n", width, height);

	unsigned char* image = SOIL_load_image(filename, &width, &height, 0,   SOIL_LOAD_RGB);

	glGenTextures(1, &(m->texture));
	glBindTexture(GL_TEXTURE_2D, m->texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
		width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);

	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glBindTexture(GL_TEXTURE_2D, 0);
	return err;
};

int loadNewModel(renderSystem * sys, char* filename){
	int err = 0;
	model M;
	size_t lenfilename = strlen(filename);
	char * name = malloc(sizeof(char)*(lenfilename+1));
	char * nameorig = name;
	if(name == NULL) {printf("ooer\n"); exit(-1);};
	//printf("%zu characters\n", sizeof(char)*lenfilename);
	strncpy(name, filename, lenfilename);
	name[lenfilename] = '\0';

	char*endname = rindex(name, '/');
	char*namepre;
	char*namepreend;
	if(endname != NULL){
		endname += 1;
		namepre = malloc(endname-name+1+256);
		strncpy(namepre, name, endname-name);
		namepre[endname-name]='\0';
		namepreend= namepre + (endname - name);
	}
	else{
		namepre=malloc(lenfilename+1+256);
		strncpy(namepre, name, lenfilename);
		namepre[lenfilename]='\0';
		namepreend=namepre+lenfilename;
	}

	char * file = file_contents(name);
	char*fileorig = file;
	char*line = NULL;

#define NUM_FILES 2
	int i = 0;
	char * names[NUM_FILES];
	while((line = strsep(&file, "\n")) != NULL){
		if(i >= 2) break;
		if (strlen(line) > 250) {printf("line in model file %s too long!\n", name); return -1;};
		strcpy(namepreend, line);
		//printf("%d, %s\n", i , namepre);
		switch(i){
			case 0:
				loadOBJmodel(&M, namepre);
				modelInitGL(&M, GL_DYNAMIC_DRAW);
				break;
			case 1:
				modelInitTex(&M, namepre);
				break;
		}
		i++;
	}

	arr_add(&(sys->models), (void*)(&M));
	array batch = arr_init(1, sizeof(mat4));

	free(nameorig);
	free(fileorig);
	free(namepre);
	return err;
};
