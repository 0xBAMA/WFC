#ifndef ENGINE
#define ENGINE

#include "includes.h"

//  ╦┌┬┐┌─┐┌─┐┌─┐
//  ║│││├─┤│ ┬├┤ 
//  ╩┴ ┴┴ ┴└─┘└─┘
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

#define N 3
#define F 0 

// forward declarations
class pattern;
class output_tile;
class wfc;


//  ╦═╗┬ ┬┬  ┌─┐
//  ╠╦╝│ ││  ├┤ 
//  ╩╚═└─┘┴─┘└─┘
class rule
{
public:
	glm::ivec2 offset;
	std::vector<int> agrees;
};

//  ╔╦╗┌─┐┌┬┐┌─┐┬  
//  ║║║│ │ ││├┤ │  
//  ╩ ╩└─┘─┴┘└─┘┴─┘
class model
{
public:
	image in;
	int dim = N;	

	void parse_input();
	void tile_dump(std::string filename);
		
	std::vector<pattern> patterns;
};

//  ╔═╗┌─┐┌┬┐┌┬┐┌─┐┬─┐┌┐┌
//  ╠═╝├─┤ │  │ ├┤ ├┬┘│││
//  ╩  ┴ ┴ ┴  ┴ └─┘┴└─┘└┘
class pattern
{
public:	

	pattern(){data.resize(N); for(auto& r: data) r.resize(N);}
		
	int count;
	std::vector<std::vector<int>> data;

	std::vector<rule> overlap_rules;
	bool agrees(glm::ivec2 offset, pattern &other);
	bool subagrees(glm::ivec2 offset, glm::ivec2 position, pattern &other);

	pattern rotate();
	pattern mirror();

	// equality operator checks all ints in the data array
	bool operator==(const pattern &other) const
	{
		for(int i = 0; i < N; i++)
			for(int j = 0; j < N; j++)
				if(this->data[i][j] != other.data[i][j])
					return false; // any difference in data violates equality

		return true; 
	}

		
	// less than operator used for sorting (compares 'count')
	bool operator< (const pattern &other) const	
	{
		if(this->count == other.count)
			return this->data[0][0] < other.data[0][0];
		else
			return this->count < other.count;
	}	
};

//  ╔═╗┬ ┬┌┬┐┌─┐┬ ┬┌┬┐  ╔╦╗┬┬  ┌─┐
//  ║ ║│ │ │ ├─┘│ │ │    ║ ││  ├┤ 
//  ╚═╝└─┘ ┴ ┴  └─┘ ┴    ╩ ┴┴─┘└─┘
class output_tile
{
public:	

};

//  ╦ ╦╔═╗╔═╗
//  ║║║╠╣ ║  
//  ╚╩╝╚  ╚═╝
class wfc
{
public:

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
