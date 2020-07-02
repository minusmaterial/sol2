#pragma once
#include <cglm/cglm.h>
#include "../graphics/GLincludes.h"
#include <ft2build.h>
#include FT_FREETYPE_H

#define N_SUPPORTED_ASCII_CHARS 128
#define FIRST_ASCII_CHAR 32


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

int initText();
int makeCharAtlas(FT_Face face);
void render_text(const char *text, float x, float y, float sx, float sy);
