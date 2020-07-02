#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <cglm/cglm.h>
#include "GLincludes.h"
#include "../callbacks.h"
#include "model.h"
#include "../utility/array.h"
#include "../ECS/entity.h"
#include "../physics/physics.h"

extern vec4 RIGHT;
extern vec4 UP;
extern vec4 FORWARD;

typedef struct {
	vec4 pos;
	vec4 forward;
	vec4 right;
	vec4 up;
	mat4 orientation;
	float fov;
	float nearval;
	float farval;
	mat4 perspective;
	mat4 view;
}camera;

typedef struct {
	int width;
	int height;
    GLFWwindow* GLFWID;
	camera cam;
}window;

typedef struct {
	window win;
	GLuint program;
	GLuint glyphProgram;
	array models;
	array batchArrays;
	camera cam;
	array renderComps;
}renderSystem;

int  initRenderer(renderSystem * output, int width, int height, char * windowName);
int modelLookup(renderSystem * Renderer, COMPTYPE ID);

camera initCam(vec4 pos,  mat4 orientation, float fov, float ratio);
int updateCam(camera * cam);
int updateCamPerspective(camera * cam, renderSystem * sys);

int batched_render_setup(renderSystem * rsys, array * positions, int model_index);
int batched_render(int model_index);

int loadNewModel(renderSystem * sys, char* filename);

void display(void);
