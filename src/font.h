#include "vars.h"

#include <ft2build.h>
#include <freetype.h>
#include <ftglyph.h>
#include <ftoutln.h>
#include <fttrigon.h>

#include <SDL_opengl.h>

#ifndef FONT_H
	#define FONT_H
	
	typedef struct ft_fontdata {
		GLuint *textures;
		GLuint listbase;
	} ft_fontdata;
	
	
	int ft_init(ft_fontdata *fontdata);
	int next_p2(int a);
	int ft_make_display_list(FT_Face face, int ch, ft_fontdata *fontdata);
	int drawText(int x, int y, float r, float g, float b, char *text, ft_fontdata *font);
#endif