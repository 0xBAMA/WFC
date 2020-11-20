#ifndef ENGINE
#define ENGINE

#include "includes.h"

class image
{
public:
	unsigned width, height;
	std::vector<unsigned char> data;

	unsigned load(std::string filename);
	int at(int x, int y);

	void count_colors();
	std::vector<glm::ivec3> colors; // only doing RGB
};


#define MAX_COLORS 16 // prevents combinatoric explosion

// used to represent a 3x3 neighborhood
struct tile
{
	// how many times have we seen this tile
	int count = 0;

	
	// the int is the number of the palette index
	// data is arranged thusly:
	//    0  1  2
	//    3  4  5
	//    6  7  8
	int data[9] = {0};

	
	// equality operator checks all 9 ints in the data array
	bool operator==(const tile &other) const
	{
		for(int i = 0; i < 9; i++)
		{ // any difference in data violates equality
			if(this->data[i] != other.data[i])
				return false; 
		}
		return true; 
	}

	
	// less than operator used for sorting (compares 'count')
	bool operator< (const tile &other) const	
	{
		return this->count < other.count; 
	}
};



class model
{
public:
	std::vector<tile> tiles;

	image in;

	// function to get all the color values from the image
	void acquire_colors();
		
	// function to grab all the tiles in the image
	//   report timing, tile count vs maximum tile count (%)
	//     eventually this will also need to rotate, mirror, etc
	void tile_parse();

	// function to add tiles
	//   also see if the tile is a repeat, if so, increment count
	void add_tile(tile t);
		
	// function to sort tiles by count
	void tile_sort();
		
	// function to dump tiles
	//  3x3 pixel opaque blocks,
	//   separated by one pixel of zero alpha
	void dump_tiles();

	std::stringstream percent_done;
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
