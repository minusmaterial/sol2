#include "shader.h"


GLuint compileShader(const char* shader_file_path, int type){
	GLuint shaderID = glCreateShader(type);

	FILE *fp;
	GLchar*shaderSource;
	
	fp = fopen(shader_file_path, "r");	
	if(fp == 0){
		printf
("ERROR: couldn't open shader source file at %s.  This shader wasn't compiled.\n", shader_file_path);
		return -1;
	}

	fseek(fp, 0L, SEEK_END);                    
	size_t size = ftell(fp);                     
	shaderSource  = calloc(1, size);       
	rewind(fp);                                
	fread(shaderSource, size, sizeof(char), fp); 
	fclose(fp);


	//free(fp);
	//printf("Compiling shader : %s\n", shader_file_path);
	//printf(shaderSource);
	const GLchar * const * shaderSP =
	 (const GLchar * const *)&shaderSource;
	glShaderSource(shaderID, 1, shaderSP, NULL);
	glCompileShader(shaderID);

	free(shaderSource);


	GLuint * Result = malloc(sizeof(GLuint));
	GLuint * InfoLogLength = malloc(sizeof(GLuint));

	//printf("Checking shader compilation\n");
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, Result);
	//printf("Checking shader compilation\n");
	glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, InfoLogLength);
	//printf("%d\n", *InfoLogLength);
	if ( *InfoLogLength > 0 ){
		GLchar * shaderErrorMessage = malloc(sizeof(GLchar)*((*InfoLogLength) + 1));
		glGetShaderInfoLog(shaderID, *InfoLogLength, NULL, shaderErrorMessage);
		printf("Compilation of Shader %s failed.  Log follows.\n", shader_file_path);
		printf("%s\n", shaderErrorMessage);
		free(shaderErrorMessage);
		return -1;
	}

	free(Result);
	free(InfoLogLength);
	//printf("Shader compiled.\n");
	return shaderID;

};

GLuint linkProgram(GLuint vertShader, GLuint fragShader){
	//printf("Linking program\n");
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, vertShader);
	glAttachShader(ProgramID, fragShader);
	glLinkProgram(ProgramID);

	int Result;
	int InfoLogLength;

	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		char * ProgramErrorMessage = malloc(sizeof(char) * (InfoLogLength + 1));
		glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, ProgramErrorMessage);
		printf("Program link failed; log follows\n%s\n", ProgramErrorMessage);
		free(ProgramErrorMessage);
		return -1;
	}

	//printf("Program linked.\n");
	glDetachShader(ProgramID, vertShader);
	glDetachShader(ProgramID, fragShader);
	glDeleteShader(vertShader);
	glDeleteShader(fragShader);

	return ProgramID;
};




