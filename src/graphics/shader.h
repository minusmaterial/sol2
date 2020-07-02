#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "render.h"

GLuint compileShader(const char * shader_file_path, int type);

GLuint linkProgram(GLuint vertShader, GLuint fragShader);
