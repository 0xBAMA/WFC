#ifndef ENGINE
#define ENGINE

#include "includes.h"

class image
{
public:
	unsigned width, height, num_colors;
	std::vector<unsigned char> data;

	unsigned load(std::string filename);
	bool count_colors();
	
};



struct tile
{
	// how many times have we seen this tile
	int count;

	// data is arranged thusly:
	//    0  1  2
	//    3  4  5
	//    6  7  8
	int[9] data;
};



class model
{
public:
	std::vector<glm::ivec3> colors;
	std::vector<tile> tiles;

	image in;

	// function to get the list of colors,
	//   store count in the image.num_colors

	// function to grab all the tiles in the image
	//   report timing, tile count vs maximum tile count (%)
	
	// function to dump tiles
	//  3x3 pixel opaque blocks,
	//   separated by one pixel of zero alpha

	// function to sort tiles by count
};




class engine
{
public:

	engine();
	~engine();

private:

	SDL_Window * window;
	SDL_GLContext GLcontext;

	ImVec4 clear_color;

	GLuint display_texture;
	GLuint display_shader;
	GLuint display_vao;
	GLuint display_vbo;

	model m;
		
	void create_window();
	void gl_setup();
	void draw_everything();

	void quit();

	bool pquit;

};

#endif
