#include "font.h"

/* Ten plik w dużej mierze jest przepisany gdzieś z Internetu...
Sam się nie znam wcale na freetype, display listach i innych rzeczach.
Ale to działa */

int next_p2(int a)
{
    int rval=1;
    while(rval<a)
        rval<<=1;
    return rval;
} 
#include <stdio.h>
int ft_init(ft_fontdata *fontdata)
{
	FT_Library ftlib;
	FT_Face ftface;
	int i;
	
	if (FT_Init_FreeType( &ftlib ))
		return 1;
	printf("font[path=%s",getConfigString("font_path"));
	if (FT_New_Face( ftlib, getConfigString("font_path"), 0, &ftface ))
		return 2;
	
	if (FT_Set_Char_Size(ftface, 0, getConfigInt("font_size")*64, getConfigInt("font_dpi"), getConfigInt("font_dpi")))
		return 3;
	
	fontdata->listbase = glGenLists(128);
	glGenTextures(128, fontdata->textures);
	
	for (i=0; i<128; i++)
		ft_make_display_list(ftface, i, fontdata);
		
	FT_Done_Face(ftface);
	
	return 0;
}

int ft_make_display_list(FT_Face face, int ch, ft_fontdata *fontdata)
{
	FT_Glyph glyph;
	FT_BitmapGlyph bitmap_glyph;
	GLubyte *expanded_data;
	int width, height;
	int i, j;
	float x,y;
	
	if (FT_Load_Glyph( face, FT_Get_Char_Index(face, ch), FT_LOAD_DEFAULT))
		return 1;
		
	if (FT_Get_Glyph(face->glyph, &glyph))
		return 1;
		
	FT_Glyph_To_Bitmap(&glyph, ft_render_mode_normal, 0, 1);
	bitmap_glyph = (FT_BitmapGlyph)glyph;

	width = next_p2(bitmap_glyph->bitmap.width);
	height = next_p2(bitmap_glyph->bitmap.rows);

	expanded_data = (GLubyte *)malloc(2*width*height*sizeof(GLubyte));

	for (j=0; j<height; j++)
	{
		for (i=0; i<width; i++)
		{
			if (i>=bitmap_glyph->bitmap.width || j>=bitmap_glyph->bitmap.rows)
				expanded_data[2*(i+j*width)]= expanded_data[2*(i+j*width)+1] = 0;
			else 
				expanded_data[2*(i+j*width)]= expanded_data[2*(i+j*width)+1] = bitmap_glyph->bitmap.buffer[i + bitmap_glyph->bitmap.width*j];
		}
	}

	glBindTexture(GL_TEXTURE_2D, fontdata->textures[ch]);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);

   glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, expanded_data );

	free(expanded_data);
	
	glNewList(fontdata->listbase+ch,GL_COMPILE);

	glBindTexture(GL_TEXTURE_2D,fontdata->textures[ch]);

	glTranslatef(bitmap_glyph->left,0,0);
	glPushMatrix();
	glTranslatef(0,bitmap_glyph->top-bitmap_glyph->bitmap.rows,0);

	x = (float)bitmap_glyph->bitmap.width / (float)width;
	y = (float)bitmap_glyph->bitmap.rows / (float)height;

	glBegin(GL_QUADS);
	glTexCoord2d(0,0); glVertex2f(0,bitmap_glyph->bitmap.rows);
	glTexCoord2d(0,y); glVertex2f(0,0);
	glTexCoord2d(x,y); glVertex2f(bitmap_glyph->bitmap.width,0);
	glTexCoord2d(x,0); glVertex2f(bitmap_glyph->bitmap.width,bitmap_glyph->bitmap.rows);
	glEnd();
	glPopMatrix();
	glTranslatef(face->glyph->advance.x >> 6 ,0,0);
	glEndList();
}

int drawText(int x, int y, float r, float g, float b, char *text, ft_fontdata *font)
{
	float modelview_matrix[16];	
	glGetFloatv(GL_MODELVIEW_MATRIX, modelview_matrix);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, getConfigInt("screen_w"), 0, getConfigInt("screen_h"));

	glColor3f(r, g, b);
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	
	glListBase(font->listbase);
	glTranslatef(x,y,0);
	glCallLists(strlen(text), GL_UNSIGNED_BYTE, text);
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);
	glPopMatrix();

	return 0;
}
