#include "objload.h"
#include "../graphics/render.h"
#include "array.h"
#include "fileload.h"

int loadOBJmodel(model * Model, const char * filename){
	int err = 0;
	array coords = arr_init(1, sizeof(GLfloat));
	array normals = arr_init(1, sizeof(GLfloat));
	array textcoords = arr_init(1, sizeof(GLfloat));
	array indices = arr_init(1, sizeof(GLuint));
	array verts = arr_init(1, sizeof(GLfloat));
	array modIndices = arr_init(1, sizeof(GLuint));

	char * storedData = (char*)file_contents(filename);	
	char * foodData = storedData;
	char * line;
	char * newl = strdup("\n");
	char * space = strdup(" ");
	char * slash = strdup("/");
	char c = 'n';

	while((line  = strsep(& foodData, newl)) != NULL)
	{
		if(strlen(line) < 2) continue;
		c = line[0];
		switch(c){
			case 'v':
				c = line[1];
				if(c == 't'){
					strsep(&line, space);
					GLfloat  a;
					a=(GLfloat)(atof(strsep(&line,space)));
					arr_add(&textcoords, (void * )&a);
					a=-(GLfloat)(atof(strsep(&line,space)));
					arr_add(&textcoords, (void * )&a);
				}
				if(c == 'n'){
					strsep(&line, space);
					GLfloat  a;
					for(int i = 0; i < 3; i++){
					a=(GLfloat)(atof(strsep(&line,space)));
					arr_add(&normals, (void * )&a);
					}
				}
				if(c == ' '){
					strsep(&line, space);
					GLfloat a;
					for(int i = 0; i < 3; i++){
					a=(GLfloat)(atof(strsep(&line,space)));
					arr_add(&coords, (void * )&a);
					}
				}
				break;
			case 'f':
					strsep(&line, space);
					GLint a;
					for(int k = 0; k < 3; k++){
						a=(GLuint)(atoi(strsep(&line,slash)));
						arr_add(&indices, (void * )&a);
						a=(GLint)(atoi(strsep(&line,slash)));
						arr_add(&indices, (void * )&a);
						a=(GLint)(atoi(strsep(&line,space)));
						arr_add(&indices, (void * )&a);
					}
				break;
			case '#':
				break;
			case ' ':
				break;
		}
	};
//Data layout:
//	P/T/N P/T/N P/T/N ...
//right now, the indices don't actually make anything more efficient.
	for(int i = 0; i < indices.size; i+= 3){

		int j = i / 3;
		arr_add(&modIndices, &j);

		arr_add(&verts, &(arr_get(GLfloat, coords, 3*(arr_get(int, indices, i)-1)+0)));
		arr_add(&verts, &(arr_get(GLfloat, coords, 3*(arr_get(int, indices, i)-1)+1)));
		arr_add(&verts, &(arr_get(GLfloat, coords, 3*(arr_get(int, indices, i)-1)+2)));

		arr_add(&verts, &(arr_get(GLfloat, textcoords, 2*(arr_get(int, indices, i+1)-1)+0)));
		arr_add(&verts, &(arr_get(GLfloat, textcoords, 2*(arr_get(int, indices, i+1)-1)+1)));

		arr_add(&verts, &(arr_get(GLfloat, normals, 3*(arr_get(int, indices, i+2)-1)+0)));
		arr_add(&verts, &(arr_get(GLfloat, normals, 3*(arr_get(int, indices, i+2)-1)+1)));
		arr_add(&verts, &(arr_get(GLfloat, normals, 3*(arr_get(int, indices, i+2)-1)+2)));
	}

/*
	printf("Vertex buffer size: %d\n", verts.size);
	for(int i = 0; i < verts.size;){
		printf("P: ");
		printf("%f ", arr_get(GLfloat, verts, i)); i++;
		printf("%f ", arr_get(GLfloat, verts, i)); i++;
		printf("%f ", arr_get(GLfloat, verts, i)); i++;

		printf("T: ");
		printf("%f ", arr_get(GLfloat, verts, i)); i++;
		printf("%f ", arr_get(GLfloat, verts, i)); i++;

		printf("N: ");
		printf("%f ", arr_get(GLfloat, verts, i)); i++;
		printf("%f ", arr_get(GLfloat, verts, i)); i++;
		printf("%f ", arr_get(GLfloat, verts, i)); i++;
		printf("\n");
	}
	*/

	Model->verts = verts;
	Model->indices = modIndices;

	free(storedData);
	free(newl);
	free(space);
	free(slash);
	arr_cleanup(&coords);
	arr_cleanup(&normals);
	arr_cleanup(&textcoords);

	return err;
};
