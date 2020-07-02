#include "callbacks.h"
#include "ECS/system.h"
#include <cglm/cglm.h>

extern systemCore * Sys;

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	Sys->Render.win.height = height;
	Sys->Render.win.width = width;
    glViewport(0, 0, width, height);
	//For some reason, glm_perspective_resize really doesn't work.  Dunno why.
	//Actually, touching the perspective matrix at all seems to cause a crash the next time glfwPollEvents, or maybe glfwSwapBuffers, is called.  I don't understand.
	//glm_perspective_resize((((float)width)/((float)height)), Sys.Render.cam.perspective);
	updateCamPerspective(&(Sys->Render.cam), &(Sys->Render));

}

//handle key presses.  Can add customised keybinds by checking for e.g. KEY_FORWARD which can be whatever GLFW keycode.  Nice and simple.
void key_callback(GLFWwindow* window, int key,  int scancode, int action, int mods){
	//this is a big ugly elif chain for now.  Something more intersting might come in the future.
	handleKey(&(Sys->Input), key, scancode, action, mods);
	if(key == GLFW_KEY_ESCAPE){
		glfwSetWindowShouldClose(Sys->Render.win.GLFWID, 1);}
	/*else 	else if(key == Sys->Input.keybinds.KEY_BACKWARD){
	else if(key == Sys->Input.keybinds.KEY_RIGHT){
	else if(key == Sys->Input.keybinds.KEY_LEFT){
	//camera zooms///////////////////////
	else if(key == GLFW_KEY_I){
		Sys->Render.cam.fov += 0.01;
		updateCamPerspective(&(Sys->Render.cam), &(Sys->Render));}
	else if(key == GLFW_KEY_O){
		Sys->Render.cam.fov -= 0.01;
		updateCamPerspective(&(Sys->Render.cam), &(Sys->Render));}*/
};
