#pragma once
#include <cglm/cglm.h>
#include "../graphics/GLincludes.h"
#include "../graphics/render.h"
#include <ft2build.h>
#include FT_FREETYPE_H

#define N_SUPPORTED_ASCII_CHARS 128
#define FIRST_ASCII_CHAR 30


typedef struct{
	vec2 advance;
	vec2 size;
	vec2 bearing;
	float tx; // x offset of glyph in texture coordinates
}charinfo;

typedef struct {
    GLfloat x;
    GLfloat y;
    GLfloat s;
    GLfloat t;
}text_point;

//defined in text.c.  Take an ascii character as a number, and use that as an
//index into this array.  At that spot is a 'character' struct corresponding.
extern charinfo char2char[];
//Also defined in text.c.  The dimensions of the character altas bitmap.
//Storing these as globals is ugly, but eh.
extern int atlas_width;
extern int atlas_height;

int initText(renderSystem * Render);
int makeCharAtlas(renderSystem * Render, FT_Face face);
void _render_text(renderSystem * render, const char *text, float x, float y, float sx, float sy);
void render_text(renderSystem * render,  float x, float y, float sx, float sy, const char *text, ...);

#define render_text(renderer, x,y,sx1,sy1,format, ...) \
	{   float tsxabcd = (float)(sx1) / ((renderer)->win.width);\
		float tsyabcd = (float)(sy1) / ((renderer)->win.height);\
		float txabcd = -1 + (float)(x) * tsxabcd ;\
	    float tyabcd = 1 - (float)(y) * tsyabcd ;\
		snprintf((renderer)->text_buffer, SCREEN_TEXT_BUFFER_SIZE, format, __VA_ARGS__); \
		_render_text((renderer), (renderer)->text_buffer, txabcd,tyabcd,tsxabcd,tsyabcd);}

