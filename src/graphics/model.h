#pragma once
#include <SOIL.h>
#include "../ECS/typedefs.h"
#include "../ECS/entity.h"
#include "GLincludes.h"
#include <cglm/cglm.h>
#include "render.h"
#include "../utility/array.h"

struct model{
	GLuint VAO;
	GLuint VBO;
	GLuint EBO;
	GLuint texture;
	array verts;
	array indices;
};
typedef struct model model;

typedef struct {
	COMPTYPE ID;
	unsigned int model;
}comp_render;

int modelInitGL(model * m, GLint draw_type);
int modelInitTex(model * m, char * filename);
