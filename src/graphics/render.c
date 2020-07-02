#include "render.h"
#include "model.h"
#include "shader.h"

int initRenderer(renderSystem * output, int width, int height, char * windowName){
	output->win.width = width;
	output->win.height = height;

	//Opengl calls
	//glfw init, better than whatever else you can use
	if( !glfwInit() )
        printf("Failed to initialise GLFW- something's fucked lads\n");

    glfwWindowHint(GLFW_SAMPLES, 4); 
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); 
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); 
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

    output->win.GLFWID = glfwCreateWindow(output->win.width, output->win.height, windowName, NULL, NULL);
	if(output->win.GLFWID == NULL){
		printf("Failed to make GLFW window!\n");
		glfwTerminate();
		exit(-1);
	};
	glfwMakeContextCurrent(output->win.GLFWID);

	//glew init
	glewExperimental=GL_TRUE;
	if(glewInit()){
		printf("GLEW initialisation Error, exiting\n");
		exit(-1);
	}
	//openGL settings
	glClearColor(0.0f,0.0f,0.5f,0.0f);
	glViewport(0,0,output->win.width - 0, output->win.height - 0);
	glEnable(GL_DEPTH_TEST);
	//setting Callbacks
	//framebuffer_size_callback comes from callbacks.h
	glfwSetFramebufferSizeCallback(output->win.GLFWID, framebuffer_size_callback); 
	//I would rather put this in initInput, as that fits better conceptually, but 
	//given that in fact all callbacks are part of GLFW (and we need the renderer
	//info) It'll have to go here. 
	glfwSetKeyCallback(output->win.GLFWID, key_callback); 

	//shader setup
	GLuint vShader  = compileShader("../build/shaders/vertexShader.txt", GL_VERTEX_SHADER);
	GLuint fShader  = compileShader("../build/shaders/fragmentShader.txt", GL_FRAGMENT_SHADER);
	output->program = linkProgram(vShader, fShader);
	//text (glyph) rendering shaders
	GLuint textvShader	= 
		compileShader("../build/shaders/glyphVertexShader.txt", 
		GL_VERTEX_SHADER);
	GLuint textfShader  = 
		compileShader("../build/shaders/glyphFragmentShader.txt", 
		GL_FRAGMENT_SHADER);
	output->glyphProgram = linkProgram(textvShader, textfShader);


	//end opengl calls
	output->models  = arr_init(1, sizeof(model));
	output->renderComps  = arr_init(1, sizeof(comp_render));
	output->batchArrays = arr_init(1, sizeof(array));
}

int modelLookup(renderSystem * Renderer, COMPTYPE ID){
	//rests on the assumption that the components are sorted by ID.
	array * arr = &(Renderer->renderComps);
	int b = arr->size-1;
	int a = 0;
	int look = 0;
	while(1){
		look = (a + b)/2;
		int res = arr_get(comp_render, (*arr), look).ID;
		if(res == ID) 
			return look;
		if(res > ID){
			b = look;
			if(b <= a) return -1;
		}
		if(res < ID){
			a = look;
			if(b <= a) return -1;
		}
	}
};

void display(void){
}

int batched_render_setup(renderSystem * rsys, array * positions, int model_index){
	assert(rsys->batchArrays.size > 0);
	assert(rsys->batchArrays.size == rsys->models.size);

	array * batch = &(arr_get(array, rsys->batchArrays, model_index));
	batch->size = 0;
	int posi = 0;
	for(int i = 0; i < rsys->renderComps.size; i++){
		if ((arr_get(comp_render, rsys->renderComps, i)).model != model_index) continue;
		COMPTYPE ID = arr_get(comp_render, rsys->renderComps, i).ID;
		while(ID!=(arr_get(comp_pos,(*positions),posi)).ID){
			 if(ID>(arr_get(comp_pos,(*positions),posi)).ID){
				printf("FUCKED UP- in batch render setup, position array ID overreach!\n");
				exit(-1);
			}
			posi++;
		};
	arr_add(batch, (void*)&(arr_get(comp_pos, (*positions), posi).transform));
	}
}

int batched_render(int model_index){

};

camera initCam(vec4 pos, mat4  orientation, float fov, float ratio){
	camera cam;

	cam.fov = fov;
	cam.nearval = 0.01f;
	cam.farval = 10000.0f;

	glm_mat4_copy(orientation, cam.orientation);
	glm_vec4_copy(pos, cam.pos);
	glm_mat4_mulv(cam.orientation, RIGHT, cam.right);
	glm_mat4_mulv(cam.orientation, FORWARD, cam.forward);
	glm_mat4_mulv(cam.orientation, UP, cam.up);

	glm_look(cam.pos, cam.forward, cam.up, cam.view);

	glm_perspective(
		cam.fov, 
		ratio,
		cam.nearval,
		cam.farval,
		cam.perspective
			);

	return cam;
}

int updateCamPerspective(camera * cam, renderSystem * sys){
	int err = 0;
	glm_perspective(
		cam->fov, 
		(float)(sys->win.width)/(float)(sys->win.height),
		cam->nearval,
		cam->farval,
		cam->perspective
			);
	return err;
};

int updateCam(camera * cam){
	int err;
	glm_mat4_mulv(cam->orientation, FORWARD, cam->forward);
	glm_mat4_mulv(cam->orientation, RIGHT, cam->right);
	glm_mat4_mulv(cam->orientation, UP, cam->up);
	glm_look(cam->pos, cam->forward, cam->up, cam->view);
	return err;
};
