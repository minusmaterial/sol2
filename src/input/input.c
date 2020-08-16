#include "input.h"

int initInput(inputSystem * insys){

	//pr2intf("Initialising input system...\n");
	//Just hardcoding keybinds for now.

	for (int i=0;i<MAX_NUM_KEYS;i++){
		insys->keybinds[i] = 0;
	}

	insys->keybinds[GLFW_KEY_W]		= KEY_FORWARD;
	insys->keybinds[GLFW_KEY_S] 	= KEY_BACKWARD;	
	insys->keybinds[GLFW_KEY_A]		= KEY_LEFT;
	insys->keybinds[GLFW_KEY_D]		= KEY_RIGHT;
	insys->keybinds[GLFW_KEY_I]		= KEY_ZOOM_IN;
	insys->keybinds[GLFW_KEY_O]		= KEY_ZOOM_OUT;
	insys->keybinds[GLFW_KEY_LEFT]	= KEY_ROT_LEFT;
	insys->keybinds[GLFW_KEY_RIGHT]	= KEY_ROT_RIGHT;
	insys->keybinds[GLFW_KEY_SPACE]	= KEY_UP;
	insys->keybinds[GLFW_KEY_C]	= KEY_DOWN;

	for (int i=0;i<MAX_NUM_INPUTS;i++){
		insys->inputs[i] = 0;
	}
}

int handleKey(inputSystem * insys, int key, int scancode, int action, int mods){
	if(action == GLFW_PRESS){
		insys->inputs[insys->keybinds[key]] = 1;
		//printf("You pressed key %d\n", key);
	}
	else if(action == GLFW_RELEASE){
		insys->inputs[insys->keybinds[key]] = 0;
		//printf("You released key %d\n", key);
	}

}
