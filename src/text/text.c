#include "text.h"


charinfo char2char[N_SUPPORTED_ASCII_CHARS];
int atlas_width;
int atlas_height;

int initText(){

	FT_Library ft;
	if (FT_Init_FreeType(&ft))
		printf("ERROR::FREETYPE: Could not init FreeType Library\n");
	FT_Face face;
	if (FT_New_Face(ft, "fonts/terminus.ttf", 0, &face))
		printf("ERROR::FREETYPE: Failed to load font\n");
	FT_Set_Pixel_Sizes(face, 0, 48);  

	makeCharAtlas(face);

	FT_Done_Face(face);
	FT_Done_FreeType(ft);
};

int makeCharAtlas(FT_Face face){
	int w = 0;
	int h = 0;
	FT_GlyphSlot g = face->glyph;

	for(int i=FIRST_ASCII_CHAR;i<N_SUPPORTED_ASCII_CHARS;i++){
		if(FT_Load_Char(face, i, FT_LOAD_RENDER)) {
			fprintf(stderr, "Loading character %c failed!\n", i);
			continue;
		  }
		w += g->bitmap.width;
		if(g->bitmap.rows > h) 
			h = g->bitmap.rows;
	}
	printf("Atlas is %d pixels wide and %d pixels high\n", w, h);
	atlas_width=w;
	atlas_height=h;

	GLuint tex;
	//glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, w, h, 0, GL_RED, GL_UNSIGNED_BYTE, 0);	

	int x = 0;

	for(int i = FIRST_ASCII_CHAR; i < 128; i++) {
		if(FT_Load_Char(face, i, FT_LOAD_RENDER)) {
			fprintf(stderr, "Loading character %c failed!\n", i);
			continue;
		  }

	  glTexSubImage2D(GL_TEXTURE_2D, 0, x, 0, g->bitmap.width, g->bitmap.rows, GL_RED, GL_UNSIGNED_BYTE, g->bitmap.buffer);

	  x += g->bitmap.width;
	  char2char[i].advance[0] = g->advance.x;
	  char2char[i].advance[1] = g->advance.y;

	  char2char[i].size[0] = g->bitmap.width;
	  char2char[i].size[1] = g->bitmap.rows;

	  char2char[i].bearing[0] = g->bitmap_left;
	  char2char[i].bearing[1] = g->bitmap_top;

	  char2char[i].tx	= (float)x/w;
	}
	
	
};



//This is horrendous code copy-pasted from the internet.  
//Vulnerable to buffer overflows from non-null-terminated strings.
//Replace as soon as possible.  God.
void render_text(const char *text, float x, float y, float sx, float sy) {
	text_point coords[6 * strlen(text)];
	charinfo * c = char2char;
	int n = 0;

	for(const char *p = text; *p; p++) { 
		float x2 =  x + c[*p].bearing[0] * sx;
		float y2 = -y - c[*p].bearing[1] * sy;
		float w = c[*p].size[0] * sx;
		float h = c[*p].size[1] * sy;

		// Advance the cursor to the start of the next character 
		x += c[*p].advance[0] * sx;
		y += c[*p].advance[1] * sy;

		// Skip glyphs that have no pixels 
		if(!w || !h)
			continue;

		coords[n++] = (text_point){x2, -y2, c[*p].tx, 0};
		coords[n++] = (text_point){x2+w, -y2    , c[*p].tx + c[*p].bearing[0] / atlas_width,   0};
		coords[n++] = (text_point){x2,     -y2 - h, c[*p].tx,                                          c[*p].bearing[1] / atlas_height}; //remember: each glyph occupies a different amount of vertical space
		coords[n++] = (text_point){x2 + w, -y2    , c[*p].tx + c[*p].bearing[0] / atlas_width,   0};
		coords[n++] = (text_point){x2,     -y2 - h, c[*p].tx,                                          c[*p].bearing[1] / atlas_height};
		coords[n++] = (text_point){x2 + w, -y2 - h, c[*p].tx + c[*p].bearing[0] / atlas_width,                 c[*p].bearing[1] / atlas_height};
	  }
		glBufferData(GL_ARRAY_BUFFER, sizeof coords, coords, GL_DYNAMIC_DRAW);
		glDrawArrays(GL_TRIANGLES, 0, n);
}
