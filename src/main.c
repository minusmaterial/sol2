#include "main.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <cglm/cglm.h>
#include <pthread.h>
#include <unistd.h>

#include "callbacks.h"

#include "graphics/GLincludes.h"
#include "graphics/render.h"
#include "graphics/shader.h"
#include "ECS/entity.h"
#include "ECS/system.h"
#include "schedule/schedule.h"
#include "text/text.h"

#include "utility/array.h"
#include "utility/fileload.h"
#include "utility/objload.h"

#include "utility/maths.h"



vec4 RIGHT 		= {1.0f, 0.0f, 0.0f, 0.0f};
vec4 UP 		= {0.0f, 1.0f, 0.0f, 0.0f};
vec4 FORWARD 	= {0.0f, 0.0f, 1.0f, 0.0f};

mat4 WORLD_TO_OPENGL = {{1.0, 0.0, 0.0, 0.0},
						{0.0, 1.0, 0.0, 0.0},
						{0.0, 0.0, -1.0, 0.0},
						{0.0, 0.0, 0.0, 1.0}};
int height = 1920;
int width = 1080;

systemCore  COREnametonotbereferencedthankyou;
systemCore * Sys = & COREnametonotbereferencedthankyou;

int main(int argc, char * argv[]){

	//this is important- it branches into initialising all of the engine's systems
	initSystems(Sys);
	initText(&Sys->Render);

	make_entity(Sys, COMP_RENDER & COMP_POSITION);
	make_entity(Sys, COMP_RENDER & COMP_POSITION);
	make_entity(Sys, COMP_RENDER & COMP_POSITION);
	printf("num_entities: %ld\n", Sys->Entity.entities.size);
	{
		mat4 matrix; glm_mat4_identity(matrix);
		//glm_rotate(matrix, M_PI, UP);
		//vec4 vec = {0.0,-2.0,0.0,1.0};
		vec4 vec = {0.0,0.0,0.0,1.0};
		Sys->Render.cam = initCam(	vec, 
									matrix, 
									M_PI/1.8,
									(float)Sys->Render.win.width / (float)Sys->Render.win.height);
	}

	vec3 model_pos = {0.0f, 0.0f, 10.0f};
	vec3 model_pos_2 = {4.0f, -1.0f, 8.0f};
	mat4 modmat;
	mat4 modmat_2;
	glm_mat4_identity(modmat);
	glm_mat4_identity(modmat_2);
	glm_translate(modmat, model_pos);
	glm_translate(modmat_2, model_pos_2);

	loadNewModel(&(Sys->Render), "assets/sphere/sphere.i");
	//loadNewModel(&(Sys->Render), "assets/hammer/hammer.i");
	//loadNewModel(&(Sys->Render), "assets/tri/tri.i");
	//loadNewModel(&(Sys->Render), "assets/shrum/shrum.i");
	//loadNewModel(&(Sys->Render), "assets/bbox/bbox.i");

	GLuint MVP_loc = glGetUniformLocation(Sys->Render.program, "MVP");
	GLuint tex_loc = glGetUniformLocation(Sys->Render.program, "textureSampler");

	//batched_render_setup(&(Sys->Render),&(arr_get(array, Sys->comparrays, intlog2(COMP_POSITION))),0);

	//loop//////////////////////
	while(!glfwWindowShouldClose(Sys->Render.win.GLFWID))
	{

		//Act based on input state.
		float speed = 0.1;
		vec4 move = GLM_VEC4_ZERO_INIT;
		if(Sys->Input.inputs[KEY_FORWARD] == 1){
			vec4 temp;
			glm_vec4_scale(	Sys->Render.cam.forward,
							(Sys->Input.inputs[KEY_FORWARD])*1,
							temp);
			glm_vec4_add(temp, move, move);
		}
		if(Sys->Input.inputs[KEY_BACKWARD] == 1){
			vec4 temp;
			glm_vec4_scale(	Sys->Render.cam.forward,
							(Sys->Input.inputs[KEY_BACKWARD])*-1,
							temp);
			glm_vec4_add(temp, move, move);
		}
		if(Sys->Input.inputs[KEY_LEFT] == 1){
			vec4 temp;
			glm_vec4_scale(	Sys->Render.cam.right,
							(Sys->Input.inputs[KEY_LEFT])*-1,
							temp);
			glm_vec4_add(temp, move, move);
		}
		if(Sys->Input.inputs[KEY_RIGHT] == 1){
			vec4 temp;
			glm_vec4_scale(	Sys->Render.cam.right,
							(Sys->Input.inputs[KEY_RIGHT])*1,
							temp);
			glm_vec4_add(temp, move, move);
		}
		if(Sys->Input.inputs[KEY_UP]){
			vec4 temp;
			glm_vec4_scale(	Sys->Render.cam.up,
							(Sys->Input.inputs[KEY_UP])*1,
							temp);
			glm_vec4_add(temp, move, move);
		}
		if(Sys->Input.inputs[KEY_DOWN]){
			vec4 temp;
			glm_vec4_scale(	Sys->Render.cam.up,
							(Sys->Input.inputs[KEY_DOWN])*-1,
							temp);
			glm_vec4_add(temp, move, move);
		}
		//printf("camera is at %f, %f, %f\n", Sys->Render.cam.pos[0], Sys->Render.cam.pos[1], Sys->Render.cam.pos[2]);
		//printf("camera forward is at %f, %f, %f\n", Sys->Render.cam.forward[0], Sys->Render.cam.forward[1], Sys->Render.cam.forward[2]);
		//printf("camera right is at %f, %f, %f\n", Sys->Render.cam.right[0], Sys->Render.cam.right[1], Sys->Render.cam.right[2]);
		//printf("camera up is at %f, %f, %f\n", Sys->Render.cam.up[0], Sys->Render.cam.up[1], Sys->Render.cam.up[2]);

		glm_vec4_scale_as(move, speed, move);
		glm_vec4_add(Sys->Render.cam.pos, move, Sys->Render.cam.pos);
		

		if(Sys->Input.inputs[KEY_ZOOM_IN] == 1){
			Sys->Render.cam.fov += 0.05;
			updateCamPerspective(&(Sys->Render.cam), &(Sys->Render));
		}
		if(Sys->Input.inputs[KEY_ZOOM_OUT] == 1){
			Sys->Render.cam.fov -= 0.05;
			updateCamPerspective(&(Sys->Render.cam), &(Sys->Render));
		}

		if(Sys->Input.inputs[KEY_ROT_RIGHT] == 1){
			glm_rotate(Sys->Render.cam.orientation, 1.0f/(1<<5), UP);		
			updateCamPerspective(&(Sys->Render.cam), &(Sys->Render));
		}
		if(Sys->Input.inputs[KEY_ROT_LEFT] == 1){
			glm_rotate(Sys->Render.cam.orientation, -1.0f/(1<<5), UP);		
		}


		//STUFF///
		updateTime(&(Sys->Schedule));
		//stuff///////////////////////
		glm_rotate(modmat, 1.0f/(1<<7), UP);	
		glm_rotate(modmat, 1.0f/(1<<7), RIGHT);	
		glm_rotate(modmat, 1.0f/(1<<7), FORWARD);	

		updateCam(&(Sys->Render.cam));

		glClearColor(0.0f,0.0f,0.08f,0.0f);
		//glClearColor(0.5f,0.5f,0.5f,0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(Sys->Render.program);

		mat4 matrix;
		glm_mat4_identity(matrix);
		glm_mat4_mulN((mat4 *[])
			{ &Sys->Render.cam.perspective, &Sys->Render.cam.view,&WORLD_TO_OPENGL, &modmat,&WORLD_TO_OPENGL }, 5, matrix);
		glUniformMatrix4fv(MVP_loc, 1, GL_FALSE, matrix[0]);


		model * mod;
		//for(int i = 0; i < Sys->Render.models.size; i++){
		for(int i = 0; i < 1; i++){
			mod = &(arr_get(model, Sys->Render.models, i));
			if(mod != NULL){
			glBindVertexArray(mod->VAO);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D,mod->texture);
			glUniform1i(glGetUniformLocation(Sys->Render.program, "tex"), 0);
			glDrawElements(GL_TRIANGLES, mod->indices.size, GL_UNSIGNED_INT, (void*)0);
			glBindVertexArray(0);
			};
		};

		glm_mat4_identity(matrix);
		glm_mat4_mulN((mat4 *[])
			{&Sys->Render.cam.perspective, &Sys->Render.cam.view,&WORLD_TO_OPENGL, &modmat_2, &WORLD_TO_OPENGL}, 5, matrix);
		glUniformMatrix4fv(MVP_loc, 1, GL_FALSE, matrix[0]);

		//for(int i = 0; i < Sys->Render.models.size; i++){
		for(int i = 0; i < 1; i++){
			mod = &(arr_get(model, Sys->Render.models, i));
			if(mod != NULL){
			glBindVertexArray(mod->VAO);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D,mod->texture);
			glUniform1i(glGetUniformLocation(Sys->Render.program, "tex"), 0);
			glDrawElements(GL_TRIANGLES, mod->indices.size, GL_UNSIGNED_INT, (void*)0);
			glBindVertexArray(0);
			};
		};

		glUseProgram(Sys->Render.glyphProgram);
		float sx = 1.5 / Sys->Render.win.width;
		float sy = 1.5 / Sys->Render.win.height;
		render_text(&Sys->Render,   8, 50, 1.5, 1.5,
					"The quick fox jumps over the lazy dog", 0);
		render_text(&Sys->Render, 
				8,  100, 2, 2,  
				"camera is at %f, %f, %f\n", 
				Sys->Render.cam.pos[0], 
				Sys->Render.cam.pos[1], 
				Sys->Render.cam.pos[2]);

		glfwPollEvents();    
    	glfwSwapBuffers(Sys->Render.win.GLFWID);
		//usleep(25000);



	}
	glfwTerminate();
	return 1;
}
