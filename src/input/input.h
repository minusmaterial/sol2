#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <GLFW/glfw3.h>
#define MAX_NUM_KEYS 512
#define MAX_NUM_INPUTS 512

enum inputtype{
	KEY_NONE,
	KEY_FORWARD,
	KEY_BACKWARD,
	KEY_LEFT,
	KEY_RIGHT,
	KEY_UP,
	KEY_DOWN,
	KEY_ZOOM_IN,
	KEY_ZOOM_OUT,
	KEY_ROT_RIGHT,
	KEY_ROT_LEFT,
};

typedef struct{
	//maps GLFW_KEY_[X] enums to KEY_[X] enums
	int keybinds[MAX_NUM_KEYS];
	//maps KEY_[X] enums to input state (0: not pressed, 1: pressed)
	int inputs[MAX_NUM_INPUTS];
}inputSystem;

int initInput(inputSystem * insys);

int handleKey(inputSystem * insys, int key, int scancode, int action, int mods);
