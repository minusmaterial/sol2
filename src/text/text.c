#include "text.h"


charinfo char2char[N_SUPPORTED_ASCII_CHARS];

int initText(renderSystem * Render){

	FT_Library ft;
	if (FT_Init_FreeType(&ft))
		printf("ERROR::FREETYPE: Could not init FreeType Library\n");
	FT_Face face;
	if (FT_New_Face(ft, "fonts/terminus.ttf", 0, &face))
	//if (FT_New_Face(ft, "fonts/dejavu.ttf", 0, &face))
		printf("ERROR::FREETYPE: Failed to load font\n");
	if (FT_Set_Pixel_Sizes(face, 0, 48)) { printf("Something went wrong with the pixel sizes.\n");}

	makeCharAtlas(Render, face);

	FT_Done_Face(face);
	FT_Done_FreeType(ft);
};

int makeCharAtlas(renderSystem * Render, FT_Face face){
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
	Render->atlas_width=w;
	Render->atlas_height=h;

	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &(Render->glyphTex));
	glBindTexture(GL_TEXTURE_2D, Render->glyphTex);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glPixelStorei(GL_PACK_ALIGNMENT, 1);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, w, h, 0, GL_RED, GL_UNSIGNED_BYTE, 0);	

	int x = 0;

	for(int i = FIRST_ASCII_CHAR; i < 128; i++) {
		if(FT_Load_Char(face, i, FT_LOAD_RENDER)) {
			fprintf(stderr, "Loading character %c failed!\n", i);
			continue;
		  }

	  glTexSubImage2D(GL_TEXTURE_2D, 0, x, 0, g->bitmap.width, g->bitmap.rows, GL_RED, GL_UNSIGNED_BYTE, g->bitmap.buffer);

	  //if((g->bitmap.width) && (g->bitmap.rows))
	  //printf("g->bitmap.buffer[0] value for character %c: %d\n", i, g->bitmap.buffer[0]);

	  char2char[i].advance[0] = (float)g->advance.x;
	  //printf("x-Advance for %c should be %ld\n.", i, g->advance.x);
	  char2char[i].advance[1] = (float)g->advance.y;
	  //printf("y-Advance for %c should be %ld\n.", i, g->advance.y);

	  char2char[i].size[0] = (float)g->bitmap.width;
	  //printf("width for %c should be %u\n.", i, g->bitmap.width);
	  char2char[i].size[1] = (float)g->bitmap.rows;
	  //printf("height for %c should be %u\n.", i, g->bitmap.rows);

	  char2char[i].bearing[0] = (float)g->bitmap_left;
	  char2char[i].bearing[1] = (float)g->bitmap_top;

	  char2char[i].tx	= (float)x/w;
	  x += g->bitmap.width;
	}
};



//This is horrendous code copy-pasted from the internet.  
//Vulnerable to buffer overflows from non-null-terminated strings.
//Replace as soon as possible.  God.
void _render_text(renderSystem * render, const char *text, float x, float y, float sx, float sy) {
	text_point coords[6 * strlen(text)];
	charinfo * c = char2char;
	int n = 0;

	for(const char *p = text; *p; p++) { 
		float x2 =  x + c[*p].bearing[0] * sx;
		float y2 = -y - c[*p].bearing[1] * sy;
		float w = c[*p].size[0] * sx;
		float h = c[*p].size[1] * sy;
		//printf("x is %f\n", x);
		//printf("y is %f\n", x);

		// Advance the cursor to the start of the next character 
		//printf("advance is %f\n", c[*p].advance[0] );
		x += c[*p].advance[0] * sx * 0.02 ;
		//y += c[*p].advance[1] * sy;

		// Skip glyphs that have no pixels 
		if(!w || !h){
			//printf("Skipping a glyph: '%c'.\n", *p);
			continue;}

		//printf("Some coords:\n", coords[n-1]);
		coords[n++] = (text_point){x2, -y2, c[*p].tx, 0};
		//printf("x:%f y:%f s:%f t:%f\n", coords[n-1].x, coords[n-1].y, coords[n-1].s, coords[n-1].y);
		coords[n++] = (text_point){x2+w, -y2, c[*p].tx + c[*p].size[0] / (float)render->atlas_width ,   0};
		coords[n++] = (text_point){x2,     -y2 - h, c[*p].tx, c[*p].size[1] / (float)render->atlas_height}; //remember: each glyph occupies a different amount of vertical space
		//printf("%u is the atlas width\n", render->atlas_width);
		//printf("%u is the atlas height\n", render->atlas_height);
		//printf("%f is the x-size\n", c[*p].size[0]) ;
		//printf("%f is the y-size\n", c[*p].size[1]) ;
		coords[n++] = (text_point){x2 + w, -y2    , c[*p].tx + c[*p].size[0] / (float)render->atlas_width,   0};
		coords[n++] = (text_point){x2,     -y2 - h, c[*p].tx, (c[*p].size[1]) / (float)render->atlas_height};
		coords[n++] = (text_point){x2 + w, -y2 - h, c[*p].tx + c[*p].size[0] / (float)render->atlas_width, c[*p].size[1] / (float)render->atlas_height};
	  }
		glBindBuffer(GL_ARRAY_BUFFER, render->glyphVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(coords), coords, GL_DYNAMIC_DRAW);
		glBindVertexArray(render->glyphVAO);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, (void*)(0*sizeof(GLfloat)));

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D,render->glyphTex);
		glUniform1i(glGetUniformLocation(render->glyphProgram, "tex"), 0);

		glDrawArrays(GL_TRIANGLES, 0, n);
		glBindVertexArray(0);
}

