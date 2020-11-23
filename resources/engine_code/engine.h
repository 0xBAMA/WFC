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

class tile;

class rule
{
	glm::ivec2 offset;
	std::vector<tile> agrees;
};


// used to represent a 3x3 neighborhood
class tile
{
public:
	// how many times have we seen this tile
	int count = 0;

	
	// the int is the number of the palette index
	// data is arranged thusly:
	//    0  1  2
	//    3  4  5
	//    6  7  8
	int data[9] = {0};

	tile rotate()
	{// return a counterclockwise rotated version of this tile
		tile temp; // only used in tile_parse(), count is ignored
		temp.data[0] = data[2]; temp.data[1] = data[5]; temp.data[2] = data[8];
		temp.data[3] = data[1]; temp.data[4] = data[4];	temp.data[5] = data[7];
		temp.data[6] = data[0];	temp.data[7] = data[3];	temp.data[8] = data[6];
		return temp;
	}
	
	tile mirror()
	{// return a mirrored version of this tile	
		tile temp;
		temp.data[0] = data[2]; temp.data[1] = data[1];	temp.data[2] = data[0];
		temp.data[3] = data[5]; temp.data[4] = data[4];	temp.data[5] = data[3];
		temp.data[6] = data[8]; temp.data[7] = data[7];	temp.data[8] = data[6];
		return temp;
	}

	int color()
	{ // return index of the center pixel's color
		return data[4];
	}

	int neighbor(int x, int y)
	{
		if(x == -1 && y == -1)
			return data[0];
		else if(x == 0 && y == -1)
			return data[1];
		else if(x == 1 && y == -1)
			return data[2];
		else if(x == -1 && y == 0)
			return data[3];
		else if(x == 0 && y == 0)
			return data[4]; // this is the same as color()
		else if(x == 1 && y == 0)
			return data[5];
		else if(x == -1 && y == 1)
			return data[6];
		else if(x == 0 && y == 1)
			return data[7];
		else if(x == 1 && y ==1)
			return data[8];
		else
			return 0; // don't do this outside of the range
	}	
	
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
		// if(this->count == other.count)
			return this->data[4] < other.data[4];
		// else
			// return this->count < other.count; 
	}

	std::vector<rule> overlap_rules;
};


class output_tile
{
public:
	output_tile(int tile_count);
	bool collapsed;
	std::vector<int> possible_tiles;

	// void collapse();
};



class model
{
public:

// -- INPUT (PARSING STEP) --
	// number of input image
	int num = 0;

	image in;

	// function to get all the color values from the image
	void acquire_colors();
		
	// function to grab all the tiles in the image
	//   report timing, tile count vs maximum tile count (%)
	//     eventually this will also need to rotate, mirror, etc
	void tile_parse();

	// function to add tiles
	void add_tile(tile t);

// -- MODEL VISUALIZATION --

	// function to dump tiles
	//  3x3 pixel opaque blocks,
	//   separated by one pixel of zero alpha
	void dump_tiles(std::string filename);

// -- OUTPUT (GENERATION STEP) --

	// function to construct the output model
	void construct_output();
		
	// function to collapse the output
	void collapse();

	// function to dump the current state of the output
	//   (averages remaining tiles to arrive at final pixel color)
	void output();
		
private:
	std::stringstream percent_done; // used to 
	std::vector<tile> tiles;
	std::vector<std::vector<output_tile>> out;

	int width = 35;
	int height = 35;

	// helper function to collapse a single cell
	// void collapse_cell(int x, int y);

	// helper function to simplify bounds checking
	bool on_board(int x, int y);

	// helper function to tell if all cells are collapsed
	bool all_collapsed();

	// helper function to get indices of lowest entropy cells
	void lowest_entropy(std::vector<glm::ivec2> &in);
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
